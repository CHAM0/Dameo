#include "color.h"
#include "coordinate.h"

#ifndef _PIECE_H_
#define _PIECE_H_

/* 
 * Represente une piece de l'echiquier 
*/ 
 
class Piece {

  private:

/* Couleur de la piece */
    Color color_ {};
/* Coordonnee de la piece */
    Coordinate coordinate_ {};

  public:
    
    Piece(): color_{colorNone}, coordinate_{} {}; 
    Piece(Color color, std::size_t x, std::size_t y): color_{color}, coordinate_{x,y} {};

    // Permet de retourner la couleur d'une piece 
    Color getColor() const {return color_;}

    // Permet de retourner l'abscisse de la piece 
    std::size_t getX() const {return coordinate_.getX();}

    // Permet de retourner l'ordonnee de la piece 
    std::size_t getY() const {return coordinate_.getY();}

    // Permet de retourner les coordonnees de la piece
    Coordinate getCoordinate() const {return coordinate_;}

    // Affiche les informations d'une piece
    void displayInfos() const {coordinate_.display(); std::cout<<color_<<std::endl;}

    // Permet de deplacer une piece 
    void moveTo(Coordinate const & newCoord) {coordinate_ = newCoord;}

    void copy(Piece* piece) {
        piece->color_ = color_;
        piece->coordinate_ = coordinate_;
    }
};

#endif