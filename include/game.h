#pragma once
#include <thread>
#include <string>
#include "board.h"
#include "display2d.h"
#include "min_max.h"
#include <chrono>
#include <fstream>

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
    void turn(std::string c1, std::string c2, std::string first);
    void play();
    void playTurn(Color const & color);
    void playTurnCursor(Color const & color);
    void launch();
    void save();
    void load();
    std::vector<std::tuple<Coordinate, Color> > getCurrentState();
    Board& getEchiquier() {return *echiquier_;}

    Coordinate getDeplacement();
};
