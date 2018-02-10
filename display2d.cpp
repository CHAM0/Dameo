#include "include/display2d.h"

Display2D::Display2D(int nbCases): window_{sf::VideoMode(size_, size_), "Dameo" /*, sf::Style::None */}, scale_{nbCases} {
    // Création du plateau
    int tableSize = size_ / scale_;
    for (int i=0; i<size_; ++i) {
        int offset = i%2;
        for (int j=0; j<size_; ++j) {
            sf::RectangleShape rect(sf::Vector2f(tableSize, tableSize));
            if(j%2 == offset) rect.setFillColor(sf::Color::Black);
            else rect.setFillColor(sf::Color::White);
            rect.setPosition(i*tableSize,j*tableSize);
            square.push_back(rect);
        }
    }
}

void Display2D::render() {

        // nettoie l'ecran 
        window_.clear(sf::Color::Black);
        displayBoard();
        displayPieces();
        displayHelp();
        // met a jour la fenetre 
        window_.display(); 

    while (window_.isOpen()) {


        sf::Event event;


        // on inspecte tous les évènements de la fenêtre qui ont été émis depuis la précédente itération
        while (window_.pollEvent(event)) {
            // évènement "fermeture demandée" : on ferme la fenêtre
            if (event.type == sf::Event::Closed) window_.close();  
            else if (event.type == sf::Event::MouseButtonPressed ) {
                mouse = Coordinate(event.mouseButton.y / (size_ /scale_), event.mouseButton.x / (size_ /scale_));
            }  
        } 

 
    }
}                                         

void Display2D::displayBoard() {
    
    int tableSize = size_ / scale_;
    for (int i=0; i<size_; ++i) {
        int offset = i%2;
        for (int j=0; j<size_; ++j) {
            sf::RectangleShape rect(sf::Vector2f(tableSize, tableSize));
            if(j%2 == offset) rect.setFillColor(sf::Color::Black);
            else rect.setFillColor(sf::Color::White);
            rect.setPosition(i*tableSize,j*tableSize);
            window_.draw(rect);
        }
    }
}

void Display2D::displayPieces() {
    for (auto & e : state_) {
        Piece2D piece(e,scale_);
        piece.displayPiece(scale_);
        pieces_.push_back(piece);
        window_.draw(piece.piece);
    }
    /*
    for (auto & p : pieces) { 
        p.displayPiece(scale_);
        window_.draw(p.piece);
    }
    */
}

void Display2D::displayHelp() {
    std::vector<Piece2D> pieces;
    for (auto & e : help_) {
        Piece2D piece(e, scale_);
        //pieces.push_back(piece);
        piece.displayPiece(scale_);
        window_.draw(piece.piece);
    }
    /*
    for (auto & p : pieces) { 
        p.displayPiece(scale_);
        window_.draw(p.piece);
    }
    */
}

void Display2D::update() {
    // nettoie l'ecran 
    window_.clear(sf::Color::Black);
    displayBoard();
    displayPieces();
    displayHelp();
    // met a jour la fenetre 
    window_.display();
}


void Display2D::updateHelp() {
    // nettoie l'ecran 
    window_.clear(sf::Color::Black);
    displayBoard();
    for(auto & e : pieces_) {
        window_.draw(e.piece);
    }
    displayHelp();
    // met a jour la fenetre 
    window_.display();
}