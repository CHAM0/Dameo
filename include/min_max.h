#pragma once
#include "board.h"
#include <vector>

#ifndef MIN_MAX_H
#define MIN_MAX_H 

class MinMax 
{
    private:

    Board & m_board;
    int m_depth {};
    const Color  m_color {};

    public:

    MinMax(Board & dameo, int depth, const Color & color);
    int eval(Board & board,const Color & color);
    int min(Board & board, int depth,const Color & color);
    int max(Board & board, int depth,const Color & color);
};

#endif