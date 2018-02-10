#include "include/game.h"


Game::Game(): window_(8) {
    std::cout<<YELLOW<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n########## DAMEO ##########"<<RESET<<std::endl;

    std::size_t size = 0;
    while(size % 8 != 0 || size == 0) {
        std::cout<<"\nTaille de la grille : ";
        std::cin>>size;
    }
    echiquier_ = new Board(size);
    echiquier_->init(colorWhite,colorBlack);

    std::cout<<GREEN<<"\n\tDebut de la partie\n"<<GREEN<<std::endl;
}

void Game::play() {
    while (1 == 1) {
        // Sauvegarde de l'etat actuel du jeux
        states_.push_back(getCurrentState());

        playTurnCursor(colorWhite);
        // Tour du joueur 1
        playTurn(colorWhite);
        playTurn(colorBlack);
    }
}

void Game::playTurn(Color const & color) {   

    // Sauvegarde de l'etat actuel du jeux
    states_.push_back(getCurrentState());
    // Affichage console
    echiquier_->displayBoard();

    std::cout<<"\nTour du joueur "<<color<<"\n "<<std::endl;
    window_.setState(getCurrentState());
    Coordinate move;
    std::vector<Coordinate> deplacement;

    // Calcul si une piece du joueur peut manger
    auto eat = getLongerEat(color);


    // Cas joueur peut manger un pion
    if (eat.size() != 0 ) {
        window_.hideHelp();
        move = getDeplacement();
        while (checkStartEat(move, eat)) {
            std::cout<<RED<<"\nImpossible de deplacer ce pion vous pouvez manger avec :\n"<<RESET<<std::endl;
            for (int i=0; i<eat.size(); i++) {
                std::cout<<std::get<0>(eat[i][0])<<":"<<std::get<1>(eat[i][0]);
                std::cout<<""<<std::endl;
            }
            move = getDeplacement();

        }

        auto startCoord = move;
        deplacement = computeEat(move, eat);
        window_.setPieceHelp(deplacement);
        std::thread th (&Display2D::update,&window_);
        th.join();

        move = getDeplacement();
        while(!checkEndEat(move,deplacement)) {
            std::cout<<RED<<"\n Impossible de deplacer ce pion a cet endroit :\n"<<RESET<<std::endl;
            move = getDeplacement();
        }

        auto path = getPath(startCoord, move, eat);
        eatPiece(startCoord, path, color);
    }

    else {
        // Dans le cas contraire
        char choix = 'n';
        while (choix != 'y') {
            // Selection coordonnee de depart
            window_.hideHelp();
            move = getDeplacement();
            deplacement = echiquier_->getDeplacements(move);
            window_.setPieceHelp(deplacement);
            std::thread th(&Display2D::update,&window_);

            // Selection coordonnee d'arrive
            std::cout<<"Jouer ce coup ? y/n :";
            std::cin>>choix;
            th.join();            
        }

        
        auto m = echiquier_->getMove(move);
        auto s = echiquier_->getSquare(m.start_);                                           
        while(!checkMove(m) || !echiquier_->checkColor(color, s)) {
            std::cout<<RED<<"Deplacement impossible ! \n"<<RESET<<std::endl;
            window_.hideHelp();
            echiquier_->displayBoard();
            auto start = getDeplacement();
            deplacement = echiquier_->getDeplacements(start);
            window_.setPieceHelp(deplacement);
            std::thread th(&Display2D::update,&window_);
            m = echiquier_->getMove(start);
            s = echiquier_->getSquare(m.start_);
            th.join();
        }

        echiquier_->movePiece(&m);

        std::cout<<GREEN<<"Coup effectue"<<RESET<<std::endl;
    }
        window_.hideHelp();
        window_.setState(getCurrentState());
        std::thread th3(&Display2D::update,&window_);
        th3.join(); 
}

void Game::playTurnCursor(Color const & color) {

    // Sauvegarde de l'etat actuel du jeux
    states_.push_back(getCurrentState());
    // Affichage console
    echiquier_->displayBoard();

    std::cout<<"\nTour du joueur "<<color<<"\n "<<std::endl;
    window_.setState(getCurrentState());
    Coordinate start;
    std::vector<Coordinate> deplacement;

    // Calcul si une piece du joueur peut manger
    auto eat = getLongerEat(color);

    while(true) {
        window_.hideHelp();
        start = window_.getMousePosition();
        deplacement = echiquier_->getDeplacements(start);
        window_.setPieceHelp(deplacement);
        window_.updateHelp();
        start.display();
        std::cout<<"start"<<std::endl;



    }
}

void Game::launch() {

    // pions de test
    //echiquier_->newPiece(colorBlack, Coordinate(4.0,3.0));
    //echiquier_->newPiece(colorBlack, Coordinate(4.0,5.0));
    //echiquier_->newPiece(colorBlack, Coordinate(3.0,2.0));
    //echiquier_->newPiece(colorBlack, Coordinate(4.0,1.0));

    std::thread test(&Game::play,this);
    window_.render();
    test.join();
}

std::vector<std::tuple<Coordinate, Color> > Game::getCurrentState() {
    auto pieces = echiquier_->getPieces();
    std::vector<std::tuple<Coordinate, Color> > state;
    for (auto const & p : pieces) {
        state.push_back(std::make_tuple(p->getCoordinate(), p->getColor()));
    }

    /*
    for (auto & s : state) {
        auto pos = std::get<0>(s);
        Color c = std::get<1>(s);
        pos.display();
        std::cout<<c<<std::endl;
        
    }
    */

    return state;
}

std::vector<std::vector<std::tuple<int, int>>> Game::getLongerEat(Color const & color) {
    int BestScore = 0;
    std::vector<std::vector<std::tuple<int, int>>> BestPath;

    int score = 0;
    std::vector<std::tuple<int, int>> path;

    auto pieces = echiquier_->getPieces(color);

    for(auto & p : pieces) {
        auto coord = p->getCoordinate();
        path.push_back(std::make_tuple(static_cast<int>(coord.getX()), static_cast<int>(coord.getY())));
        recursivity(coord, score, path, color, BestScore, BestPath);
        path.clear();
    }

    std::cout<<"BestEat : "<<BestScore<<std::endl;
    if (BestScore == 0) BestPath.clear();
    return BestPath;
}

void Game::recursivity(Coordinate & coord, int score, std::vector<std::tuple<int, int>> path, Color const & color, int & BestScore, std::vector<std::vector<std::tuple<int, int>>> & BestPath) {
    // std::cout<<"recursivite ";
    // coord.display();
    // std::cout<<" : ";
    auto availableDirections = echiquier_->getAvailableDirections(coord);

    for (auto & d : availableDirections) {

        echiquier_->ejectPiece(coord.add(std::get<0>(d),std::get<1>(d)));

        auto nextCoord = coord.add(2 * std::get<0>(d), 2 * std::get<1>(d));
        auto nextScore = score + 1;
        path.push_back(d);
        recursivity(nextCoord, nextScore, path, color,BestScore, BestPath);

        if(color == colorWhite) echiquier_->newPiece(colorBlack,coord.add(std::get<0>(d),std::get<1>(d)));
        else if (color == colorBlack) echiquier_->newPiece(colorBlack,coord.add(std::get<0>(d),std::get<1>(d)));
        path.pop_back();
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

bool Game::checkStartEat(Coordinate const & coord, std::vector<std::vector<std::tuple<int, int>>> & path) {
    for (int i=0; i<path.size(); i++) {
            if (static_cast<int>(coord.getX()) != std::get<0>(path[i][0]) && static_cast<int>(coord.getY()) != std::get<1>(path[i][0])) return true;
    }
    return false;
}

std::vector<std::tuple<int, int>> Game::getPath(Coordinate const & startCoord, Coordinate & endCoord, std::vector<std::vector<std::tuple<int, int>>> & path) {
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

bool Game::checkEndEat(Coordinate const & coord, std::vector<Coordinate> & path) {
    for (auto &d : path) {
        if (d == coord) return true;
    }
    return false;
}

std::vector<Coordinate> Game::computeEat(Coordinate const & coord, std::vector<std::vector<std::tuple<int, int>>> & path) {
   std::vector<Coordinate> coordList;
   for (int i=0; i<path.size(); i++) {
        int x = static_cast<int>(coord.getX());
        int y = static_cast<int>(coord.getY());
        for (int j=1; j<path[i].size(); j++) {
            x += 2 * std::get<0>(path[i][j]);
            y += 2 * std::get<1>(path[i][j]);
        }
        std::cout<<x<<":"<<y<<std::endl;
        coordList.push_back(Coordinate(x,y));
    }
    return coordList;
}

Coordinate Game::getDeplacement() {
    std::size_t coordX;
    std::cout<< "Ordonnee de la piece a deplacer : ";
    std::cin>>coordX;

    std::size_t coordY;
    std::cout<< "Abscisse de la piece a deplacer : ";
    std::cin>>coordY;

    return Coordinate(coordX, coordY);
}

bool Game::checkMove(Move & move) {
    if (!move.start_.isValid(echiquier_->getSize())) {
        std::cout<<RED<<"Coordonnee de depart invalide"<<RESET<<std::endl;
        return false;
    }
    else if (!move.finish_.isValid(echiquier_->getSize())) {
        std::cout<<RED<<"Coordonnee de fin invalide"<<RESET<<std::endl;
        return false;
    }
    
    auto deplacements = echiquier_->getDeplacements(move.start_);
        for (auto &d : deplacements) {
            d.display();
        }
        std::cout<<""<<std::endl;   
        for (auto &d : deplacements) {
            if (d == move.finish_) return true;
        }
                                                 
    return false;
}

void Game::eatPiece(Coordinate & coord, std::vector<std::tuple<int, int>> & path, Color const & color) {
    echiquier_->ejectPiece(coord);
    int x = static_cast<int>(coord.getX());
    int y = static_cast<int>(coord.getY());
    for (int i=1; i<path.size(); i++) {
        x += std::get<0>(path[i]);
        y += std::get<1>(path[i]);
        echiquier_->ejectPiece(Coordinate(x,y));
        x += std::get<0>(path[i]);
        y += std::get<1>(path[i]);
    }
    echiquier_->newPiece(color, Coordinate(x,y));
}

