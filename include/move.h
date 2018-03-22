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
/* Chemin du deplacement */
    std::vector<std::tuple<int, int>> path_{};

    Move(): start_{}, finish_{} {};
    Move(Coordinate & start, Coordinate & finish): start_(start), finish_(finish) {};
    Move(Coordinate & start, Coordinate & finish, std::vector<std::tuple<int, int>> & path ): start_(start), finish_(finish), path_(path) {};
    void show() {
        //std::cout<<"("<<start_.getX()<<":"<<start_.getY()<<")";
        //std::cout<<" -> ("<<finish_.getX()<<":"<<finish_.getY()<<")"<<std::endl;
    };
    
};

#endif