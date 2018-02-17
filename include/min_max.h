#include "game.h"

#include <vector>

#ifndef MIN_MAX_H
#define MIN_MAX_H 

class MinMax 
{
    private:

    int m_depth {};
    std::vector<Coordinate> m_deplacements {};

    public:

    MinMax(Game board, int depth);

};

#endif