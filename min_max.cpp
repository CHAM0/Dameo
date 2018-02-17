#include "include/min_max.h"

MinMax::MinMax(Board & dameo, int depth, const Color & color) : m_depth{depth}, m_board{dameo}, m_color{color} {
    auto deplacements = dameo.getAvailableMoves(color);
    int maxValue = -1000;
    Move bestMove;

    for (auto & d : deplacements) {

        dameo.movePiece(d,color);
        auto colorinverse = colorNone;
        if (color == colorWhite) colorinverse = colorBlack;
        else colorinverse = colorWhite;
        int value = min(dameo,depth - 1,colorinverse);

        if(value > maxValue) {
            maxValue = value;
            bestMove = d;
        }

        dameo.movePieceInverse(d,color);
    
    }

    std::cout<<"Mouvement choisit : ";
    bestMove.show();
    dameo.movePiece(bestMove,color);
}

int MinMax::min(Board & board, int depth,const Color & color) {
    if (depth == 0) {
        return eval(board,color);
    }

    auto deplacements = board.getAvailableMoves(color);
    int minValue = 1000;

    for (auto & d : deplacements) {

        board.movePiece(d,color);  
        int value = max(board,depth - 1,color);

        if(value < minValue) {
            minValue = value;
        }

        board.movePieceInverse(d,color);
        //m_board = board;
    }

    return minValue;
}
    
int MinMax::max(Board  & board, int depth,const Color & color) {
    if (depth == 0) {
        return eval(board,color);
    }

    auto deplacements = board.getAvailableMoves(color);
    int maxValue = -1000;

    for (auto & d : deplacements) {

        board.movePiece(d,color);  
        int value = min(board,depth - 1,color);

        if(value > maxValue) {
            maxValue = value;
        }

        board.movePieceInverse(d,color);
        //m_board = board;
    }

    return maxValue;
}

int MinMax::eval(Board  & board,const Color & color) {
    auto pieceJ1 = board.getPieces(color);
    std::vector<Piece*> pieceJ2;
    if (color == colorWhite) pieceJ2 = board.getPieces(colorBlack);
    else pieceJ2 = board.getPieces(colorWhite);

    return pieceJ1.size() - pieceJ2.size();
}
