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
        playTurnCursor(colorBlack);
        // Tour du joueur 1
        //playTurn(colorWhite);
        //playTurn(colorBlack);
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
    auto eat = echiquier_->getLongerEat(color);

    //test
    //auto moves = getAvailableMoves(color);
    //echiquier_->movePiece(moves[1],color);
    //echiquier_->displayBoard();
    //echiquier_->movePieceInverse(moves[1],color);
    //echiquier_->displayBoard();

    // Cas joueur peut manger un pion
    if (eat.size() != 0 ) {
        window_.hideHelp();
        move = echiquier_->getDeplacement();
        while (echiquier_->checkStartEat(move, eat)) {
            std::cout<<RED<<"\nImpossible de deplacer ce pion vous pouvez manger avec :\n"<<RESET<<std::endl;
            for (int i=0; i<eat.size(); i++) {
                std::cout<<std::get<0>(eat[i][0])<<":"<<std::get<1>(eat[i][0]);
                std::cout<<""<<std::endl;
            }
            move = echiquier_->getDeplacement();

        }

        auto startCoord = move;
        deplacement = echiquier_->computeEat(move, eat);
        window_.setPieceHelp(deplacement);
    
        move = echiquier_->getDeplacement();
        while(!echiquier_->checkEndEat(move,deplacement)) {
            std::cout<<RED<<"\n Impossible de deplacer ce pion a cet endroit :\n"<<RESET<<std::endl;
            move = echiquier_->getDeplacement();
        }

        auto path = echiquier_->getPath(startCoord, move, eat);
        echiquier_->eatPiece(startCoord, path, color);
    }

    else {
        // Dans le cas contraire
        char choix = 'n';
        while (choix != 'y') {
            // Selection coordonnee de depart
            window_.hideHelp();
            move = echiquier_->getDeplacement();
            deplacement = echiquier_->getDeplacements(move);
            window_.setPieceHelp(deplacement);

            // Selection coordonnee d'arrive
            std::cout<<"Jouer ce coup ? y/n :";
            std::cin>>choix;          
        }

        
        auto m = echiquier_->getMove(move);
        auto s = echiquier_->getSquare(m.start_);                                           
        while(!echiquier_->checkMove(m) || !echiquier_->checkColor(color, s)) {
            std::cout<<RED<<"Deplacement impossible ! \n"<<RESET<<std::endl;
            window_.hideHelp();
            echiquier_->displayBoard();
            auto start = echiquier_->getDeplacement();
            deplacement = echiquier_->getDeplacements(start);
            window_.setPieceHelp(deplacement);
            m = echiquier_->getMove(start);
            s = echiquier_->getSquare(m.start_);
        }

        echiquier_->movePiece(m,color);

        std::cout<<GREEN<<"Coup effectue"<<RESET<<std::endl;
    }
        window_.hideHelp();
        window_.setState(getCurrentState());

}

void Game::playTurnCursor(Color const & color) {
    // Sauvegarde de l'etat actuel du jeux
    states_.push_back(getCurrentState());
    // Affichage console
    echiquier_->displayBoard();

    std::cout<<"\nTour du joueur "<<color<<"\n "<<std::endl;
    window_.setState(getCurrentState());


    using namespace std::literals;
    std::this_thread::sleep_for(1s);
    //window_.update();

    MinMax test(*echiquier_, 3, color);

    /*
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
    */


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
