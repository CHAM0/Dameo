#include "include/min_max.h"

MinMax::MinMax(Board & dameo, int depth, const Color & color) : m_depth{depth}, m_board{dameo}, m_color{color} {
    auto deplacements = dameo.getAvailableMoves(color);
    int maxValue = -1000;
    Move bestMove;

    Board cpBoard(8);

        //test copie echiquier
        dameo.copy(cpBoard);
    for (auto & d : deplacements) {

        

        cpBoard.movePiece(d,color);
        //dameo.displayBoard();

        auto colorinverse = colorNone;
        if (color == colorWhite) colorinverse = colorBlack;
        else colorinverse = colorWhite;

        int value = min(cpBoard,depth - 1,colorinverse);

        if(value > maxValue || ( ( value == maxValue) && (rand()%2 == 0) ) ) {
            maxValue = value;
            bestMove = d;
        }

        dameo.copy(cpBoard);
        //dameo.movePieceInverse(d,color);
        //dameo.displayBoard();
    
    }
    std::cout<<"Mouvement choisit : ";
    bestMove.show();
    dameo.movePiece(bestMove,color);
    //m_bestMove = bestMove;
}

int MinMax::min(Board & board, int depth,const Color & color) {
    auto colorinverse = colorNone;
    if (color == colorWhite) colorinverse = colorBlack;
    else colorinverse = colorWhite; 
    
    if (depth == 0) {
        return eval(board,colorinverse);
    }

    auto deplacements = board.getAvailableMoves(color);
    int minValue = 1000;

    Board cpBoard(8);
        //test copie echiquier
        board.copy(cpBoard);

    for (auto & d : deplacements) {

        cpBoard.movePiece(d,color);          
        //board.displayBoard();
       
        int value = max(cpBoard,depth - 1,colorinverse);

        if(value < minValue || ( ( value == minValue) && (rand()%2 == 0) )) {
            minValue = value;
        }

        board.copy(cpBoard);

        //board.movePieceInverse(d,color);
        //board.displayBoard();
        //m_board = board;
    }

    return minValue;
}
    
int MinMax::max(Board &  board, int depth,const Color & color) {
    if (depth == 0) {
        return eval(board,color);
    }

    auto deplacements = board.getAvailableMoves(color);
    int maxValue = -1000;

    //test copie echiquier
    Board cpBoard(8);
    board.copy(cpBoard);

    for (auto & d : deplacements) {

        cpBoard.movePiece(d,color);
       // board.displayBoard();
        
        auto colorinverse = colorNone;
        if (color == colorWhite) colorinverse = colorBlack;
        else colorinverse = colorWhite;

        int value = min(cpBoard,depth - 1,colorinverse);

        if(value > maxValue) {
            maxValue = value;
        }

        board.copy(cpBoard);
        //board.movePieceInverse(d,color);
        //board.displayBoard();
        //m_board = board;
    }

    return maxValue;
}

int MinMax::eval(Board & board,const Color & color) {
    auto pieceJ1 = board.getPieces(color);
    std::vector<Piece*> pieceJ2;
    if (color == colorWhite) pieceJ2 = board.getPieces(colorBlack);
    else pieceJ2 = board.getPieces(colorWhite);

    return pieceJ1.size() - pieceJ2.size();
}
