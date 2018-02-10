#include "include/board.h"

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

void Board::newPiece(Color color, Coordinate const & coord) {
    Piece *tmp = new Piece(color, coord.getX(), coord.getY());
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

void Board::movePiece(Move * move) {
    //move->show();
    auto squareStart = getSquare(move->start_);
    auto squareFinish = getSquare(move->finish_);
    squareStart->piece->moveTo(move->finish_);
    ejectPiece(move->finish_);
    squareFinish->piece = squareStart->piece,
    squareStart->piece = new Piece(colorNone,squareStart->coordinate.getX(), squareStart->coordinate.getY());

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

std::vector<std::tuple<int, int>> Board::getAvailableDirections(Coordinate & coord) {
    std::vector<std::tuple<int, int>> directions;
    auto s = getSquare(coord);
    auto piece = s->piece;
    auto nextCoord = coord;
    auto nextSquare = getSquare(nextCoord);

    nextCoord.add(1.0,0.0);
    if (nextCoord.isValid(size_)) {
        nextSquare = getSquare(nextCoord);  
        if (!checkColor(piece->getColor(), nextSquare)){
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
    nextCoord.add(-1.0,0.0);
    if (nextCoord.isValid(size_)) {
        nextSquare = getSquare(nextCoord);  
        if (!checkColor(piece->getColor(), nextSquare)){
            if (!checkColor(colorNone, nextSquare)) {
                nextCoord.add(-1.0,0.0);
                if (nextCoord.isValid(size_)) {
                    nextSquare = getSquare(nextCoord);
                    if (checkColor(colorNone, nextSquare)) directions.push_back(std::make_tuple(-1,0));
                }
            }
        }     
    }

    nextCoord = coord;
    nextCoord.add(0.0,1.0);
    if (nextCoord.isValid(size_)) {
        nextSquare = getSquare(nextCoord);  
        if (!checkColor(piece->getColor(), nextSquare)){
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
    nextCoord.add(0.0,-1.0);
    if (nextCoord.isValid(size_)) {
        nextSquare = getSquare(nextCoord);  
        if (!checkColor(piece->getColor(), nextSquare)){
            if (!checkColor(colorNone, nextSquare)) {
                nextCoord.add(0.0,-1.0);
                if (nextCoord.isValid(size_)) {
                    nextSquare = getSquare(nextCoord);
                    if (checkColor(colorNone, nextSquare)) directions.push_back(std::make_tuple(0,-1));
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

    // deplace a gauche 
    nextCoord = coord;
    nextCoord.add(0.0,1.0);
    if (nextCoord.isValid(size_)) {
        nextSquare = getSquare(nextCoord);
        if (checkColor(colorNone, nextSquare)) deplacements.push_back(nextCoord);
        /*
        else {
                auto c = jumpPiece(*piece,0.0, 1.0);
                if (c.isValid(size_)) deplacements.push_back(c);
        }
        */
    } 

    // deplace a droite 
    nextCoord.subb(0.0,2.0);
    if (nextCoord.isValid(size_)) {
        nextSquare = getSquare(nextCoord);
        if (checkColor(colorNone, nextSquare)) deplacements.push_back(nextCoord);
        /*
        else {
                auto c = jumpPiece(*piece,0.0, -1.0);
                if (c.isValid(size_)) deplacements.push_back(c);
        }
        */
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

