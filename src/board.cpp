#include "../include/board.h"

Board::Board(std::size_t size) {
    size_ = size;
    for (std::size_t i=0; i<size; ++i) {
        for (std::size_t j=0; j<size; ++j) {
           Square tmp(i,j);
           board_.push_back(tmp);
           newPiece(colorNone,Coordinate(i,j));
        }
    }

}

void Board::displayBoard() {
    std::cout<<""<<std::endl;
    int cpt = 0;
    std::cout<<"   ";
    for (int i=0; i<size_; i++) std::cout<<BLUE<< " "<<i<<"  "<<RESET;
    std::cout<<" "<<std::endl;
    std::cout<<"   ";
    for (int i=0; i<size_; i++) std::cout<<BLUE<< " #  "<<RESET;
    std::cout<<" "<<std::endl;

    int cpt2 = 0;
    for (auto & q : board_) {
        if (cpt2 == 0) {
            std::cout<<BLUE<<cpt2<<" "<<RESET<<BLUE<< "#"<<RESET;
            cpt2++;
        } 
        if (cpt >= size_) {
            std::cout<<"\n"<<std::endl;
            cpt = 0;
            if (cpt == 0) {
                std::cout<<BLUE<<cpt2<<" "<<RESET<<BLUE<< "#"<<RESET;
                cpt2++;
            } 
        } 
        if (q.piece->getColor() == 0) std::cout<<YELLOW<<" +  "<<RESET;
        else {
            if (q.piece->getColor() == 1) std::cout<<WHITE<<" "<< q.piece->getColor()<<"  "<<RESET;
            else std::cout<<BLACK<<" "<< q.piece->getColor()<<"  "<<RESET;
        }
        cpt++;
    }
    std::cout<<"\n"<<std::endl;
}

void Board::displayLogs() {
    std::cout<<""<<std::endl;
    for (auto q : board_) {
        std::cout<<"Case ["<< q.coordinate.getX() << ":"<< q.coordinate.getY() <<"]"<<" couleur "<< q.color << " -> ";
        std::cout << "Pions : couleur "<< q.piece->getColor()<< " X:"<< q.piece->getX() << " Y:"<< q.piece->getY() << std::endl;
    }
    std::cout<<"\n"<<std::endl;
} 

void Board::newPiece(Color color, Coordinate const & coord, bool king) {
    Piece *tmp = new Piece(color, coord.getX(), coord.getY(), king);
    auto square = getSquare(coord);
    square->piece = tmp;

}

void Board::init(Color colorJ1,Color colorJ2) {
    for (std::size_t i=0; i< (size_ /2)-1; ++i) {
        for (std::size_t j=0; j< size_; j++) {
            if (size_ - j <= i ) {
                continue;
            }
            else if (j >= i) {
            newPiece(colorJ1,Coordinate(i,j));           
            }
        }
    }


    for (std::size_t i= size_ -1; i > (size_ /2); --i) {
        for (std::size_t j=0; j< size_; ++j) {
            if (size_ - j >= i + 2) {
                continue;
            }
            else if (j <= i) {
            newPiece(colorJ2,Coordinate(i,j));           
            }
        }
    }
}

void Board::ejectPiece(Coordinate const & coord) {
    auto square = getSquare(coord);
    //coord.display();
    delete square->piece;
    square->piece = new Piece(colorNone, coord.getX(), coord.getY());
}

void Board::movePiece(Move & move, const Color & color) {
    
    // verifie si un rois est deplace 
    auto square = getSquare(move.start_);
    bool king = square->piece->getKing();


    ejectPiece(move.start_);
    if (move.path_.size() != 0){
        int x = static_cast<int>(move.start_.getX());
        int y = static_cast<int>(move.start_.getY());
        for (int i=1; i<move.path_.size(); i++) {
            x += std::get<0>(move.path_[i]);
            y += std::get<1>(move.path_[i]);
            ejectPiece(Coordinate(x,y));
            x += std::get<0>(move.path_[i]);
            y += std::get<1>(move.path_[i]);
        }
    }

    if (king) newPiece(color,move.finish_, king);
    else newPiece(color,move.finish_);
}

void Board::movePieceInverse(Move & move, const Color & color) {
    
    std::cout<<"move inverse";
    move.show();
    std::cout<<"eject piece :"<<move.finish_.getX()<<move.finish_.getY();
    ejectPiece(move.finish_);
    if (move.path_.size() != 0){
        int x = static_cast<int>(move.finish_.getX());
        int y = static_cast<int>(move.finish_.getY());
        for (int i=move.path_.size()-1; i>0; i--) {
            x -= std::get<0>(move.path_[i]);
            y -= std::get<1>(move.path_[i]);
            std::cout<<"new piece :"<<x<<y;
            if (color == colorWhite) newPiece(colorBlack,Coordinate(x,y));
            else if (color == colorBlack) newPiece(colorWhite,Coordinate(x,y));
            else std::cin>>x;
            x -= std::get<0>(move.path_[i]);
            y -= std::get<1>(move.path_[i]);
        }
    }
    std::cout<<"new piece :"<<move.finish_.getX()<<move.finish_.getY();
    newPiece(color,move.start_);
}


Move  Board::getMove(Coordinate & coord) {
    
    Coordinate start = coord;
    std::size_t coordX;
    std::size_t coordY;
    std::cout<< "\nOrdonnee de la position d'arriver : ";
    std::cin>>coordX;
    std::cout<< "Abscisse de la position d'arriver : ";
    std::cin>>coordY;
    std::cout<<""<<std::endl;

    Coordinate finish(coordX,coordY);

    // Move  *move = new Move(start, finish);
    // move->show();

    return  Move(start,finish);
}

std::vector<std::tuple<int, int>> Board::getAvailableDirections(Coordinate & coord, Color color) {
    std::vector<std::tuple<int, int>> directions;
    auto s = getSquare(coord);
    auto piece = s->piece;
    auto nextCoord = coord;
    auto nextSquare = getSquare(nextCoord);
    bool continuer = true;

    if (piece->getKing()) {
        while (continuer) {
            nextCoord.add(0.0,1.0);
            if (nextCoord.isValid(size_)) {
                nextSquare = getSquare(nextCoord);
                if (!checkColor(colorNone,nextSquare)) continuer = false;
            }
            else continuer = false;
        }
    }
    else {
        nextCoord.add(0.0,1.0);
        nextSquare = getSquare(nextCoord);
    }
    if (nextCoord.isValid(size_)){
        if (!checkColor(color, nextSquare)){
            if (!checkColor(colorNone, nextSquare)) {
                nextCoord.add(0.0,1.0);
                if (nextCoord.isValid(size_)) {
                    nextSquare = getSquare(nextCoord);
                    if (checkColor(colorNone, nextSquare)) directions.push_back(std::make_tuple(0,1));
                }
            }
        }
    }

    nextCoord = coord;
    continuer = true;
    if (piece->getKing()) {
        while (continuer) {
            nextCoord.add(0.0,-1.0);
            if (nextCoord.isValid(size_)) {
                nextSquare = getSquare(nextCoord);
                if (!checkColor(colorNone,nextSquare)) continuer = false;
            }
            else continuer = false;
        }
    }
    else {
        nextCoord.add(0.0,-1.0);
        nextSquare = getSquare(nextCoord);
    }
    if (nextCoord.isValid(size_)){
        if (!checkColor(color, nextSquare)){
            if (!checkColor(colorNone, nextSquare)) {
                nextCoord.add(0.0,-1.0);
                if (nextCoord.isValid(size_)) {
                    nextSquare = getSquare(nextCoord);
                    if (checkColor(colorNone, nextSquare)) directions.push_back(std::make_tuple(0,-1));
                }
            }
        }
    }


    nextCoord = coord;
    continuer = true;
    if (piece->getKing()) {
        while (continuer) {
            nextCoord.add(1.0,0.0);
            if (nextCoord.isValid(size_)) {
                nextSquare = getSquare(nextCoord);
                if (!checkColor(colorNone,nextSquare)) continuer = false;
            }
            else continuer = false;
        }
    }
    else {
        nextCoord.add(1.0,0.0);
        nextSquare = getSquare(nextCoord);
    }
    if (nextCoord.isValid(size_)){
        if (!checkColor(color, nextSquare)){
            if (!checkColor(colorNone, nextSquare)) {
                nextCoord.add(1.0,0.0);
                if (nextCoord.isValid(size_)) {
                    nextSquare = getSquare(nextCoord);
                    if (checkColor(colorNone, nextSquare)) directions.push_back(std::make_tuple(1,0));
                }
            }
        }
    }



    nextCoord = coord;
    continuer = true;
    if (piece->getKing()) {
        while (continuer) {
            nextCoord.add(-1.0,0.0);
            if (nextCoord.isValid(size_)) {
                nextSquare = getSquare(nextCoord);
                if (!checkColor(colorNone,nextSquare)) continuer = false;
            }
            else continuer = false;
        }
    }
    else {
        nextCoord.add(-1.0,0.0);
        nextSquare = getSquare(nextCoord);
    }
    if (nextCoord.isValid(size_)){
        if (!checkColor(color, nextSquare)){
            if (!checkColor(colorNone, nextSquare)) {
                nextCoord.add(-1.0,0.0);
                if (nextCoord.isValid(size_)) {
                    nextSquare = getSquare(nextCoord);
                    if (checkColor(colorNone, nextSquare)) directions.push_back(std::make_tuple(-1,0));
                }
            }
        }
    }
 

    return directions;
}


Coordinate  Board::jumpPiece(Piece & piece, double x, double y) {
    auto coord = piece.getCoordinate();
    auto square = getSquare(coord);
    while (checkColor(piece.getColor(), square)) {
        // coord.display();
        // std::cout<<"test"<<piece.getColor()<<':'<<square->piece->getColor()<<std::endl;
         coord.add(x,y);
        if (coord.isValid(size_)) {
            square = getSquare(coord);
        } 
            else break;
    }
    if (checkColor(colorNone, square)) return square->getCoordinate();
    return Coordinate(-1,-1);
}

std::vector<Coordinate> Board::getDeplacements(Coordinate & coord) {
    std::vector<Coordinate> deplacements;
    auto s = getSquare(coord);
    auto piece = s->piece;
    if (!piece->getKing()){
        auto nextCoord = coord;
        auto nextSquare = getSquare(nextCoord);
        
        // deplacement en avant 
        if(piece->getColor()== 1) {
            nextCoord.add(1.0,0.0);
            if (nextCoord.isValid(size_)) {
                nextSquare = getSquare(nextCoord);  
                if (checkColor(colorNone, nextSquare)) deplacements.push_back(nextCoord);
                else {
                    auto c = jumpPiece(*piece,1.0, 0.0);
                    if (c.isValid(size_)) deplacements.push_back(c);
                }
            }
        }
        else {
            nextCoord.subb(1.0,0.0);
            if (nextCoord.isValid(size_)) {
                nextSquare = getSquare(nextCoord);  
                if (checkColor(colorNone, nextSquare)) deplacements.push_back(nextCoord);
                else {
                    auto c = jumpPiece(*piece,-1.0, 0.0);
                    if (c.isValid(size_)) deplacements.push_back(c);
                }
            }            
        }

        // deplacement diagonale droite
        nextCoord.add(0.0, 1.0);
        if (nextCoord.isValid(size_)) {
            nextSquare = getSquare(nextCoord);
            if (checkColor(colorNone, nextSquare)) deplacements.push_back(nextCoord);
            else {
                    if(piece->getColor()== 1) {
                        auto c = jumpPiece(*piece,1.0, 1.0);
                        if (c.isValid(size_)) deplacements.push_back(c);
                    }
                    else {
                        auto c = jumpPiece(*piece,-1.0, 1.0);
                        if (c.isValid(size_)) deplacements.push_back(c);
                    } 
            }
        }    

        // deplacement diagonale gauche 
        nextCoord.subb(0.0, 2.0);
        if (nextCoord.isValid(size_)) {
            nextSquare = getSquare(nextCoord);
            if (checkColor(colorNone, nextSquare)) deplacements.push_back(nextCoord);
            else {
                if(piece->getColor()== 1) {
                    auto c = jumpPiece(*piece,1.0, -1.0);
                    if (c.isValid(size_)) deplacements.push_back(c);
                }
                else {
                    auto c = jumpPiece(*piece,-1.0, -1.0);
                    if (c.isValid(size_)) deplacements.push_back(c);
                } 
            }
        }
    }

    //Deplacement rois
    else {
    
    // deplacement en avant 
        auto nextCoord = coord;
        auto nextSquare = getSquare(nextCoord);
        bool continuer = true;
        while (continuer) {
            nextCoord.add(1.0,0.0);
            if (nextCoord.isValid(size_)){
                nextSquare = getSquare(nextCoord);  
                if (checkColor(colorNone, nextSquare)) deplacements.push_back(nextCoord);
                else {
                    continuer = false;
                }
            }
            else continuer=false;
        }
    // deplacement en arriere 
        nextCoord = coord;
        continuer = true;
        while(continuer){
            nextCoord.add(-1.0,0.0);
            if (nextCoord.isValid(size_)) {
                nextSquare = getSquare(nextCoord);  
                if (checkColor(colorNone, nextSquare)) deplacements.push_back(nextCoord);
                else {
                    continuer = false;
                }
            }
            else continuer=false;
        }      

    // deplacement droite
        nextCoord = coord;
        continuer = true;
        while(continuer){
            nextCoord.add(0.0,1.0);
            if (nextCoord.isValid(size_)) {
                nextSquare = getSquare(nextCoord);  
                if (checkColor(colorNone, nextSquare)) deplacements.push_back(nextCoord);
                else {
                    continuer = false;
                }
            } 
            else continuer=false;  
        }  

        // deplacement gauche
        nextCoord = coord;
        continuer = true;
        while(continuer){
            nextCoord.add(0.0,-1.0);
            if (nextCoord.isValid(size_)) {
                nextSquare = getSquare(nextCoord);  
                if (checkColor(colorNone, nextSquare)) deplacements.push_back(nextCoord);
                else {
                    continuer = false;
                }
            }   
            else continuer=false;
        }   

    // deplacement diagonale 
    nextCoord = coord;
    continuer = true;
    while(continuer){
        nextCoord.add(1.0, 1.0);
        if (nextCoord.isValid(size_)) {
            nextSquare = getSquare(nextCoord);
            if (checkColor(colorNone, nextSquare)) deplacements.push_back(nextCoord);
            else {
                continuer = false;
            }
        } 
        else continuer=false;  
    }

    // deplacement diagonale 
    nextCoord = coord;
    continuer = true;
    while(continuer){
        nextCoord.add(-1.0, -1.0);
        if (nextCoord.isValid(size_)) {
            nextSquare = getSquare(nextCoord);
            if (checkColor(colorNone, nextSquare)) deplacements.push_back(nextCoord);
            else {
                continuer = false;
            }
        }  
        else continuer=false;  
    }

    // deplacement diagonale 
    nextCoord = coord;
    continuer = true;
    while(continuer){
        nextCoord.add(-1.0, 1.0);
        if (nextCoord.isValid(size_)) {
            nextSquare = getSquare(nextCoord);
            if (checkColor(colorNone, nextSquare)) deplacements.push_back(nextCoord);
            else {
                continuer = false;
            }
        }  
        else continuer=false;  
    }
        // deplacement diagonale
    nextCoord = coord;
    continuer = true;
    while(continuer){
        nextCoord.add(1.0, -1.0);
        if (nextCoord.isValid(size_)) {
            nextSquare = getSquare(nextCoord);
            if (checkColor(colorNone, nextSquare)) deplacements.push_back(nextCoord);
            else {
                continuer = false;
            }
        }
        else continuer=false;    
    }


 }

    return deplacements;
}

bool Board::checkColor(Color color, Square * square) {
    if ( square->piece->getColor() == color) return true;
    return false;
}

std::vector<Piece*> Board::getPieces(Color const & color) {
    std::vector<Piece*> piece;
    for (auto s : board_) {
        if(color == 0) {
            if (s.piece->getColor() != 0) piece.push_back(s.piece);
        }
        else {
            if (s.piece->getColor() == color) piece.push_back(s.piece);
        }  
    }
    return piece;
}

std::vector<std::vector<std::tuple<int, int>>> Board::getLongerEat(Color const & color) {
    int BestScore = 0;
    std::vector<std::vector<std::tuple<int, int>>> BestPath;

    int score = 0;
    std::vector<std::tuple<int, int>> path;  

    auto pieces = getPieces(color);

    for(auto & p : pieces) {
        auto coord = p->getCoordinate();
        path.push_back(std::make_tuple(static_cast<int>(coord.getX()), static_cast<int>(coord.getY())));
        //recursivity king
        if (coord.isValid(size_)) recursivity(coord, score, path, color, BestScore, BestPath);
        path.clear();
    }

    //std::cout<<"BestEat : "<<BestScore<<std::endl;
    if (BestScore == 0) BestPath.clear();
    return BestPath;
}

void Board::recursivity(Coordinate & coord, int score, std::vector<std::tuple<int, int>> path, Color const & color, int & BestScore, std::vector<std::vector<std::tuple<int, int>>> & BestPath) {
    //coord.display();
    // std::cout<<" : ";
    auto availableDirections = getAvailableDirections(coord,color);

    auto currentPiece = getSquare(coord)->piece;
    if (currentPiece->getKing() == true) {
    // king

    //std::cout<<"king recursivity"<<std::endl;

        Color oppositeColor = colorWhite;
        if (color == colorWhite) oppositeColor = colorBlack;
        for (auto & d : availableDirections) {

            auto oppNextCoord = coord.add(std::get<0>(d), std::get<1>(d));

            while ( getSquare(oppNextCoord)->piece->getColor() != oppositeColor) {
                oppNextCoord = oppNextCoord.add(std::get<0>(d), std::get<1>(d));
            }
            
            bool king = getSquare(oppNextCoord)->piece->getKing();
            ejectPiece(oppNextCoord);

            auto nextCoord = oppNextCoord.add(std::get<0>(d), std::get<1>(d));
            auto nextScore = score + 1;
            
            path.push_back(d);
            recursivity(nextCoord, nextScore, path, color,BestScore, BestPath);

            if(color == colorWhite) newPiece(colorBlack, oppNextCoord);
            else newPiece(colorWhite,oppNextCoord, king);
            path.pop_back();
        }
    }

    else {
    // pas king
        for (auto & d : availableDirections) {

            bool king = getSquare(coord.add(std::get<0>(d),std::get<1>(d)))->piece->getKing();
            ejectPiece(coord.add(std::get<0>(d),std::get<1>(d)));

            auto nextCoord = coord.add(2 * std::get<0>(d), 2 * std::get<1>(d));
            auto nextScore = score + 1;
            path.push_back(d);
            recursivity(nextCoord, nextScore, path, color,BestScore, BestPath);

            if(color == colorWhite) newPiece(colorBlack,coord.add(std::get<0>(d),std::get<1>(d)));
            else newPiece(colorWhite,coord.add(std::get<0>(d),std::get<1>(d)),king);
            path.pop_back();
        }
    }

    if (score > BestScore) {
        // std::cout<<" > ";
        BestScore = score;
        BestPath.clear();
        BestPath.push_back(path);
    }
    else if (score == BestScore) {
        // std::cout<<" == ";
        BestPath.push_back(path);

    }

    // std::cout<<" "<<std::endl;
}

bool Board::checkStartEat(Coordinate const & coord, std::vector<std::vector<std::tuple<int, int>>> & path) {
    for (int i=0; i<path.size(); i++) {
            if (static_cast<int>(coord.getX()) != std::get<0>(path[i][0]) && static_cast<int>(coord.getY()) != std::get<1>(path[i][0])) return true;
    }
    return false;
}

std::vector<std::tuple<int, int>> Board::getPath(Coordinate const & startCoord, Coordinate & endCoord, std::vector<std::vector<std::tuple<int, int>>> & path) {
   for (int i=0; i<path.size(); i++) {
        int x = static_cast<int>(startCoord.getX());
        int y = static_cast<int>(startCoord.getY());
        for (int j=1; j<path[i].size(); j++) {
            x += 2 * std::get<0>(path[i][j]);
            y += 2 * std::get<1>(path[i][j]);
        }
        if (std::get<0>(path[i][0]) == static_cast<int>(startCoord.getX()) && std::get<1>(path[i][0]) == static_cast<int>(startCoord.getY())) {
            if (x == static_cast<int>(endCoord.getX()) && y == static_cast<int>(endCoord.getY())){
                return path[i];
            }
        }
    }
    std::vector<std::tuple<int,int>> empty;
    return empty;
}

bool Board::checkEndEat(Coordinate const & coord, std::vector<Coordinate> & path) {
    for (auto &d : path) {
        if (d == coord) return true;
    }
    return false;
}

std::vector<Coordinate> Board::computeEat(Coordinate const & coord, std::vector<std::vector<std::tuple<int, int>>> & path) {
   std::vector<Coordinate> coordList;
   for (int i=0; i<path.size(); i++) {
        int x = static_cast<int>(coord.getX());
        int y = static_cast<int>(coord.getY());
        for (int j=1; j<path[i].size(); j++) {
            x += 2 * std::get<0>(path[i][j]);
            y += 2 * std::get<1>(path[i][j]);
        }
        //std::cout<<x<<":"<<y<<std::endl;
        coordList.push_back(Coordinate(x,y));
    }
    return coordList;
}

Coordinate Board::getDeplacement() {
    std::size_t coordX;
    std::cout<< "Ordonnee de la piece a deplacer : ";
    std::cin>>coordX;

    std::size_t coordY;
    std::cout<< "Abscisse de la piece a deplacer : ";
    std::cin>>coordY;

    return Coordinate(coordX, coordY);
}

bool Board::checkMove(Move & move) {
    if (!move.start_.isValid(getSize())) {
        std::cout<<RED<<"Coordonnee de depart invalide"<<RESET<<std::endl;
        return false;
    }
    else if (!move.finish_.isValid(getSize())) {
        std::cout<<RED<<"Coordonnee de fin invalide"<<RESET<<std::endl;
        return false;
    }
                                                     
    return true;
}

void Board::eatPiece(Coordinate & coord, std::vector<std::tuple<int, int>> & path, Color const & color) {
    bool king = getSquare(coord)->piece->getKing();
    ejectPiece(coord);
    int x = static_cast<int>(coord.getX());
    int y = static_cast<int>(coord.getY());
    for (int i=1; i<path.size(); i++) {
        x += std::get<0>(path[i]);
        y += std::get<1>(path[i]);
        ejectPiece(Coordinate(x,y));
        x += std::get<0>(path[i]);
        y += std::get<1>(path[i]);
    }
    newPiece(color, Coordinate(x,y),king);
}



std::vector<Move> Board::getAvailableMoves(const Color & color) {
    std::vector<Move> moves;

    // Liste des moves dans le cas ou le joueur peut manger un pion
    auto eats = getLongerEat(color);
    if (eats.size() != 0) {
        for (int i=0; i<eats.size(); i++) {
            Coordinate start(std::get<0>(eats[i][0]), std::get<1>(eats[i][0]));
            auto deplacements = computeEat(start, eats);
    
                auto path = getPath(start,deplacements[i],eats);
                Move move(start,deplacements[i],path);
                if (checkMove(move)) moves.push_back(Move(start,deplacements[i],path));
        }
        for (auto m : moves) m.show();

        return moves;
    }

    // Liste des moves dans le cas ou le joueur ne peut pas manger de pion 
    else {
        auto pieces = getPieces(color);
        for (auto & p : pieces) {
            auto start = p->getCoordinate();
            auto deplacements = getDeplacements(start); 
            for (auto & end : deplacements) {
                Move move(start,end);
                if (checkMove(move)) moves.push_back(move);
            }
        }
    }
    //std::cout<<"Listes des moves "<<std::endl;
    //for (auto m : moves) m.show();

    return moves;
}

bool Board::gameOver(Color color) {
    auto pieces = getPieces(color);
    if (pieces.size() == 0) return true ;
    return false;
}