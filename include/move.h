#include <iostream>

#include "coordinate.h"

#ifndef _MOVE_H_
#define _MOVE_H_

/*
 * Represente un deplacement
*/ 

struct Move {

/* Coordonnee de depart */
    Coordinate start_ {};
/* Coordonnee de fin */
    Coordinate finish_ {};

    Move(): start_{}, finish_{} {};
    Move(Coordinate start, Coordinate finish): start_(start), finish_(finish) {};
    void show() {
        std::cout<<"("<<start_.getX()<<":"<<start_.getY()<<")";
        std::cout<<" -> ("<<finish_.getX()<<":"<<finish_.getY()<<")"<<std::endl;
    };
    
};

#endif