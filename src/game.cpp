#include "../include/game.h"


Game::Game(): window_(8) {
    std::cout<<YELLOW<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n########## DAMEO ##########"<<RESET<<std::endl;

    std::size_t size = 0;
    while(size % 8 != 0 || size == 0) {
        std::cout<<"\nTaille de la grille : ";
        std::cin>>size;
    }
    echiquier_ = new Board(size);
    echiquier_->init(colorWhite,colorBlack);

}

void Game::play() {

    int choix = 0;

    while (choix < 1 || choix > 8) {  
        std::cout<<"\t# Menu # "<<std::endl;
        std::cout<<"1- Reprendre partie en cours"<<std::endl;
        std::cout<<"2- Nouvelle partie P1 vs P2"<<std::endl;
        std::cout<<"3- Nouvelle partie P1 vs IA (facile)"<<std::endl;
        std::cout<<"4- Nouvelle partie P1 vs IA (moyen)"<<std::endl;
        std::cout<<"5- Nouvelle partie P1 vs IA (difficile)"<<std::endl;
        std::cout<<"6- Nouvelle partie IA vs IA"<<std::endl;
    
        std::cout<<"choix : ";
        std::cin>>choix;
    }

    switch (choix){
        case 1: std::cout<<"reprise partie"<<std::endl;
        case 2: {
                std::cout<<"J1 ou J2 commence ? : "<<std::endl;
                std::string first;
                std::cout<<"choix : ";
                std::cin>>first;
                if (first == "J2") turn("p1", "p2", "J2");
                else turn("p1", "p2", "J1");
        }
        case 3: turn("p1", "ia2", "J1");
        case 4: turn("p1", "ia2", "J1");
        case 5: turn("p1", "ia2", "J1");
        case 6: turn("ia1", "ia2", "J1");

    }

    std::cout<<"\n\nFin de partie"<<std::endl;
    window_.setState(getCurrentState());
}

void Game::turn(std::string c1, std::string c2, std::string first) {

  if (first == "J2") {
    while (1==1){

    //sauvegarde etat du jeux
    states_.push_back(getCurrentState());

    if (c2 == "p2") playTurn(colorBlack);
    else if (c2 == "ia2") playTurnCursor(colorBlack);

    if (echiquier_->gameOver(colorWhite)) {
        std::cout<<"Joueur 2 a gagne"<<std::endl;
        return;
    }

    if (c1 == "p1") playTurn(colorWhite);
    else if (c1 == "ia1") playTurnCursor(colorWhite);

    if (echiquier_->gameOver(colorBlack)) {
        std::cout<<"Joueur 1 a gagne"<<std::endl;
        return;
    }

    }
  }

  else {
    while (1==1){

        //sauvegarde etat du jeux
        states_.push_back(getCurrentState());

        if (c1 == "p1") playTurn(colorWhite);
        else if (c1 == "ia1") playTurnCursor(colorWhite);

        if (echiquier_->gameOver(colorBlack)) {
            std::cout<<"Joueur 1 a gagne"<<std::endl;
            return;
        }

        if (c2 == "p2") playTurn(colorBlack);
        else if (c2 == "ia2") playTurnCursor(colorBlack);

        if (echiquier_->gameOver(colorWhite)) {
            std::cout<<"Joueur 2 a gagne"<<std::endl;
            return;
        }

    }
  }

}

void Game::playTurn(Color const & color) {   

    // Sauvegarde de l'etat actuel du jeux
    states_.push_back(getCurrentState());
    // Affichage console
    echiquier_->displayBoard();

    //Menu
    char menu = 'n';
    std::cout<<"Entrer dans le menu ? y/n : "<<std::endl;
    std::cout<<"choix : ";
    std::cin>>menu;

    if (menu == 'y') {
        std::cout<<"1- Sauvegarder la partie"<<std::endl;
        std::cout<<"2- Echanger les couleurs"<<std::endl;
        std::cout<<"3- sortir du menu"<<std::endl;

        std::cout<<"choix : ";
        std::cin>>menu;        
    }

    std::cout<<"\nTour du joueur "<<color<<"\n "<<std::endl;
    window_.setState(getCurrentState());
    Coordinate move;
    std::vector<Coordinate> deplacement;

    // Calcul si une piece du joueur peut manger
    auto eat = echiquier_->getLongerEat(color);

    // Cas joueur peut manger un pion
    if (eat.size() != 0 ) {
        window_.hideHelp();
        move = getDeplacement();
        while (echiquier_->checkStartEat(move, eat)) {
            std::cout<<RED<<"\nImpossible de deplacer ce pion vous pouvez manger avec :\n"<<RESET<<std::endl;
            for (int i=0; i<eat.size(); i++) {
                std::cout<<std::get<0>(eat[i][0])<<":"<<std::get<1>(eat[i][0]);
                std::cout<<""<<std::endl;
            }
            move = getDeplacement();

        }

        auto startCoord = move;
        deplacement = echiquier_->computeEat(move, eat);
        window_.setPieceHelp(deplacement);
    
        move = getDeplacement();
        while(!echiquier_->checkEndEat(move,deplacement)) {
            std::cout<<RED<<"\n Impossible de deplacer ce pion a cet endroit :\n"<<RESET<<std::endl;
            move = getDeplacement();
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
            move = getDeplacement();
            deplacement = echiquier_->getDeplacements(move);
            for (auto & d : deplacement)d.display();
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
            auto start = getDeplacement();
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
    //auto move = test.m_bestMove;
    //echiquier_->movePiece(move,color);
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
    //echiquier_->newPiece(colorWhite, Coordinate(3.0,0.0), true);
    //echiquier_->newPiece(colorBlack, Coordinate(3.0,1.0));
    //echiquier_->newPiece(colorBlack, Coordinate(3.0,3.0));
    //echiquier_->newPiece(colorBlack, Coordinate(4.0,1.0));


    if(echiquier_->getSize() == 8) {
        std::thread thread(&Game::play,this);
        window_.render();
        thread.join();

    }

    else play();
}

std::vector<std::tuple<Coordinate, Color> > Game::getCurrentState() {
    auto pieces = echiquier_->getPieces();
    std::vector<std::tuple<Coordinate, Color> > state;
    for (auto const & p : pieces) {
        state.push_back(std::make_tuple(p->getCoordinate(), p->getColor()));
    }

    return state;
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