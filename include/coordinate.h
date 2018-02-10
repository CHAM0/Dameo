#include <iostream>

#ifndef _COORDINATE_H_
#define _COORDINATE_H_

/* 
 * Represente une position dans un espace en deux dimensions
 * 
 * Elle permet faire la distinction entre une case
 * (qui représente la partie "physique" de l'échiquier) et une
 * position (qui n'est qu'une information de "localisation").
*/

class Coordinate {

  private:

/* L'abscisse de la coordonnee */
    std::size_t x_ {};
/* L'ordonnee de la coordonne */ 
    std::size_t y_ {};

  public:

    Coordinate(): x_(-1), y_(-1) {};
    Coordinate(std::size_t x, std::size_t y): x_{x}, y_{y} {};

    // Permet de recuperer l'abscisse 
    std::size_t getX() const {return x_;}

    // Permet de recuperer l'ordonnee
    std::size_t getY() const {return y_;}

    // Permet de verifier si la coordonnee d'un point est valide
    bool isValid(std::size_t size)  const {
        if (x_ >= size || x_ < 0.0 || y_ >= size || y_ < 0.0) return false;
        return true;
    }

    void add(double x, double y ) {
        x_ = x_ + x;
        y_ = y_ + y;
    }

    Coordinate add(int x, int y ) {return Coordinate(x_ + x, y_ + y);}

    void subb(std::size_t x, std::size_t y ) {
        x_ = x_ - x;
        y_ = y_ - y;
    }


    void display() const {std::cout<<"[ "<<x_<<":"<<y_<<" ]";}

    // Permet l'affectation de deux coordonnees 
    void operator = (Coordinate const & second) {
    x_ = second.getX();
    y_ = second.getY();
    } 


};

// Permet la comparaison par egalite de deux coordonnees
inline bool operator == (Coordinate const & first, Coordinate const & second) {
    return first.getX() == second.getX() && first.getY() == second.getY();
}

// Permet l'addition de deux coordonnees 
inline Coordinate operator + (Coordinate const & first, Coordinate const & second) {
    Coordinate tmp(first.getX() + second.getX(), first.getY() + second.getY());
    return tmp;
} 



#endif