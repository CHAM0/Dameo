#include <thread>

#include "board.h"
#include "display2d.h"

/*
 * 
*/ 

class Game {

  private:

    Board  * echiquier_;
    std::vector<std::vector<std::tuple<Coordinate, Color> > > states_;
    Display2D window_;


  public:
  
    Game();

    void play();
    void playTurn(Color const & color);
    void playTurnCursor(Color const & color);
    void launch();
    std::vector<std::vector<std::tuple<int, int>>> getLongerEat(Color const & color); 
    void recursivity(Coordinate & coord, int score, std::vector<std::tuple<int, int>> path, Color const & color, int & BestScore, std::vector<std::vector<std::tuple<int, int>>> & BestPath);
    std::vector<std::tuple<Coordinate, Color> > getCurrentState();
    bool checkStartEat(Coordinate const & coord, std::vector<std::vector<std::tuple<int, int>>> & path);
    bool checkEndEat(Coordinate const & coord, std::vector<Coordinate> & path);
    std::vector<std::tuple<int, int>> getPath(Coordinate const & startCoord, Coordinate & endCoord, std::vector<std::vector<std::tuple<int, int>>> & path);
    std::vector<Coordinate> computeEat(Coordinate const & coord, std::vector<std::vector<std::tuple<int, int>>> & path);
    // Recupere une entree
    Coordinate getDeplacement();
    // Verifie un mouvement
    bool checkMove(Move & move);
    void eatPiece(Coordinate & coord, std::vector<std::tuple<int, int>> & path, Color const & color);

};
