#include <vector>

#include "piece.h"
#include "move.h"

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Represente l'echiquier sous la forme d'un tableau de case
*/

class Board {

    /*
     * Represente chaque case de l'echiquier par une structure 
    */

    struct Square {

    /* Couleur d'une case */
        Color color;
    /* Pointeur vers une piece */
        Piece * piece {};
    /* Coordonnee de la case */
        Coordinate coordinate{};

        Square(std::size_t indexX, std::size_t indexY): color(indexY %2==0 ? colorWhite : colorBlack ), piece{nullptr}, coordinate{indexX, indexY} {};
        Coordinate getCoordinate() {return Coordinate(coordinate.getX(), coordinate.getY());}
    };

  private:

/* Tableau de case (echiquier) */
    std::vector<Square> board_;
/* Taille de l'echiquier */
    std::size_t size_;

  public:
    
    Board(std::size_t size);

    // fonction d'affichage 
    void displayBoard();
    void displayLogs();

    // retourne la taille de l'echiquier
    std::size_t getSize() {return size_;}

    // Permet d'ajouter une nouvelle piece sur l'echiquier
    void newPiece(Color color, Coordinate const & coordinate);

    // Initialise l'echiquier 
    void init(Color colorJ1, Color colorJ2);  

    // Deplace une piece 
    void movePiece(Move * move); 

    // Ejecte une piece de l'echiquier
    void ejectPiece(Coordinate const & coordonate); 

    // Recupere un mouvement
    Move getMove(Coordinate & coord);

    // Verifie la couleur d'une piece  
    bool checkColor(Color color, Square * square);

    // Permet de recuperer toutes les pieces
    std::vector<Piece*> getPieces(Color const & color = colorNone);

    // Permet de recuperer la liste des coups possibles
    std::vector<Coordinate> getDeplacements(Coordinate & coord);

    // Fonction recursive pour le deplacement a travers ses pions
    Coordinate jumpPiece(Piece & piece, double x, double y);

    // Permet de recuperer l'indice de la case en fonction des coordonnees 
    Square * getSquare(Coordinate const & coord){
        if (coord.isValid(size_)) {
            for (auto & s : board_) {
                if ( s.coordinate == coord) return &s;
        
             }
        }
        return nullptr;
    }

    // Permet de recuperer la direction d'un eat
    std::vector<std::tuple<int, int>> getAvailableDirections(Coordinate & coord);


};

#endif