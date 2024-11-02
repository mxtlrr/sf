#ifndef __MAP_HPP
#define __MAP_HPP

// #pragma once

#include <iostream> // min
#include <vector>   // ...
#include <math.h>   // floor, abs

#include "blitz.hpp" // bbRand / other stuff from the blitz engine
#include "backend/room.hpp"

namespace Map {

  constexpr int MAP_WIDTH   = 18;
  constexpr int MAP_HEIGHT  = 18;
  constexpr int MT_SIZE     = 19;
  constexpr int ZONE_AMOUNT = 3;

  extern int mapTemp[MAP_WIDTH+1][MAP_HEIGHT+1];
  extern int room1Amount[3];
  extern int room2Amount[3];
  extern int room2cAmount[3];
  extern int room3Amount[3];
  extern int room4Amount[3];

  extern std::vector<std::vector<std::string>> mapRoom;
  extern std::vector<Room> rooms;

  /* Functions */
  void createMap(int seed);
  void countRooms();

  int getZone(int y);
  int getHorizontalConnections(int map[][19], int x, int y);
  int getVerticalConnections(int map[][19], int x, int y);
  int getConnections(int map[][19], int x, int y);


  void enrichRoom1s();
  void enrichRoom2c4s();

  void defineRooms();
  void generateMaze();
  void createRooms();


  void setRoom(std::string roomName, int type,
    int pos, int min, int max);

};

// We don't want to add stuff to a seed that we've already
// generated, so after we do our operations then we want to
// clean it up.
void clearVector();

#endif