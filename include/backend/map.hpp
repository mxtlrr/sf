#ifndef __MAP_HPP
#define __MAP_HPP

// #pragma once

#include <iostream> // min
#include <math.h>   // floor


#include "blitz.hpp"

namespace Map {
  extern int ROOM1;
  extern int ROOM2;
  extern int ROOM2C;
  extern int ROOM3;
  extern int ROOM4;

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


  /* Functions */
  void createMap(int seed);
  void countRooms();

  int getZone(int y);
  int getHorizontalConnections(int map[][19], int x, int y);
  int getVerticalConnections(int map[][19], int x, int y);
  int getConnections(int map[][19], int x, int y);


  void enrichRoom1s();
  void enrichRoom2c4s();
};

#endif