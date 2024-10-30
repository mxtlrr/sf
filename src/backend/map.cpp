#include "backend/map.hpp"

namespace Map {
  int ROOM1  = 1;
  int ROOM2  = 2;
  int ROOM2C = 3;
  int ROOM3  = 4;
  int ROOM4  = 5;

  int mapTemp[MAP_WIDTH+1][MAP_HEIGHT+1];
  int room1Amount[3];
  int room2Amount[3];
  int room2cAmount[3];
  int room3Amount[3];
  int room4Amount[3];
};


void Map::createMap(int seed){
  bbSeedRnd(seed);
  countRooms();

  enrichRoom1s();
  enrichRoom2c4s();

  std::string mapRoom[Map::ROOM4+1][room2Amount[0]+room2Amount[1]+room2Amount[2]+3];
  std::cout << room2Amount[0]+room2Amount[1]+room2Amount[2]+3 << std::endl;

}

void Map::countRooms(){
  int zone;
  for(int y = 1; y <= Map::MAP_HEIGHT; y++){
    zone = Map::getZone(y);
    for(int x = 1; x <= Map::MAP_WIDTH - 1; x++){
      if (mapTemp[x][y] > 0) {
        if (mapTemp[x][y] < 255){
          mapTemp[x][y] = Map::getConnections(mapTemp, x, y);
          switch (mapTemp[x][y]) {
            case 1:
              room1Amount[zone]++;
              break;
            case 2:
              if (Map::getHorizontalConnections(mapTemp, x, y) == 2)
                room2Amount[zone]++;
              else if (Map::getVerticalConnections(mapTemp, x, y) == 2)
                room2Amount[zone]++;
              else room2cAmount[zone]++;
              break;
            case 3:
              room3Amount[zone]++;
              break;
            case 4:
              room4Amount[zone]++;
              break;
          }
        }
      }
    }
  }
}


////////////////////////////////////////////

int Map::getZone(int y){
  return (int)std::min(Map::ZONE_AMOUNT - 1,
      (int)(floor((double)(Map::MAP_WIDTH-y)/Map::MAP_WIDTH *
      Map::ZONE_AMOUNT)));
}

int Map::getHorizontalConnections(int map[][19], int x, int y){
  return std::min(1, map[x+1][y]) + std::min(1, map[x-1][y]);
}

int Map::getVerticalConnections(int map[][19], int x, int y){
  return std::min(1, map[x][y+1]) + std::min(1, map[x][y-1]);
}

int Map::getConnections(int map[][19], int x, int y){
  return Map::getVerticalConnections(map,x,y) + getHorizontalConnections(map,x,y);
}


/////////////////// room enrichment
////////// actual spaghetti code!

void Map::enrichRoom1s(){
  int x2=0; int y2=0; int roomsLeft;
  for(int i = 0; i <= 2; i++){
    roomsLeft = 5 - room1Amount[i];
    if(roomsLeft > 0){
      for(int y = (Map::MAP_HEIGHT / ZONE_AMOUNT) * (2-i) + 1;
        y <= ((Map::MAP_HEIGHT / ZONE_AMOUNT) * ((2-i)+1)) -2;
        y++){
        for(int x = 2; x <= Map::MAP_WIDTH-2; x++){
          if(mapTemp[x][y] == 0){
            if(getConnections(mapTemp, x, y) == 1){
              if(mapTemp[x+1][y] != 0){
                x2 = x + 1;
                y2 = y;
              } else if(mapTemp[x-1][y] != 0){
                x2 = x - 1;
                y2 = y;
              } else if(mapTemp[x][y+1] != 0){
                x2 = x;
                y2 = y + 1;
              } else if(mapTemp[x][y-1] != 0){
                x2 = x;
                y2 = y - 1;
              }

              bool placed = false;
              if(mapTemp[x2][y2] > 1 && mapTemp[x2][y2] < 4){
                switch(mapTemp[x2][y2]){
                  case 2:
                    if(getHorizontalConnections(mapTemp, x2, y2) == 2){
                      room2Amount[i]--;
                      room3Amount[i]++;
                      placed = true;
                    } else if(getVerticalConnections(mapTemp, x2, y2) == 2){
                      room2Amount[i]--;
                      room3Amount[i]++;
                      placed = true;
                    }
                    break;
                  
                  case 3:
                    room3Amount[i]--;
                    room4Amount[i]++;
                    placed = true;
                    break;
                }

                if(placed){
                  mapTemp[x2][y2]++;
                  mapTemp[x][y] = 1;
                  room1Amount[i]++;
                  roomsLeft--;
                }
              }
            }
          }

          if(roomsLeft == 0) break;
        }

        if(roomsLeft == 0) break;
      }
    }
  }
}

void Map::enrichRoom2c4s(){
  int zoneStart = 0; int zoneEnd = 0;
  bool placed;
  for(int i = 0; i <= 2; i++){
    switch(i){
      case 2:
        zoneStart = 2;
        zoneEnd = Map::MAP_HEIGHT / 3;
        break;
      case 1:
        zoneStart = Map::MAP_HEIGHT / 3 + 1;
        zoneEnd = (int)(Map::MAP_HEIGHT * (2.0/3.0) - 1);
        break;
      case 0:
        zoneStart = (int)(Map::MAP_HEIGHT * (2.0/3.0) + 1);
        zoneEnd = Map::MAP_HEIGHT - 2;
        break;
    }

    if(room4Amount[i] < 1){
      printf("ROOM4  forced in zone %d\n", i);
      placed = false;

      for(int y = zoneStart; y <= zoneEnd; y++){
        for(int x = 2; x <= Map::MAP_WIDTH - 2; x++){
          if(mapTemp[x][y] == 3){
            if(mapTemp[x+1][y] == 0 && mapTemp[x+1][y+1] == 0 && mapTemp[x+2][y] == 0){
              mapTemp[x + 1][y] = 1;
              placed = true;
            } else if (mapTemp[x - 1][y] == 0 && mapTemp[x - 1][y + 1] == 0 && mapTemp[x - 1][y - 1] == 0 && mapTemp[x - 2][y] == 0) {
              mapTemp[x - 1][y] = 1;
              placed = true;
            } else if (mapTemp[x][y + 1] == 0 && mapTemp[x + 1][y + 1] == 0 && mapTemp[x - 1][y + 1] == 0 && mapTemp[x][y + 2] == 0) {
              mapTemp[x][y + 1] = 1;
              placed = true;
            } else if (mapTemp[x][y - 1] == 0 && mapTemp[x + 1][y - 1] == 0 && mapTemp[x - 1][y - 1] == 0 && mapTemp[x][y - 2] == 0) {
              mapTemp[x][y - 1] = 1;
              placed = true;
            }

            if(placed) {
              mapTemp[x][y] = 4; // room 4 now!
              printf("ROOM4 forced into (%d, %d)\n", x, y);
              room4Amount[i]++;
              room3Amount[i]--;
              room1Amount[i]++;
            }
          }

          if(placed) break;
        }
        if(placed) break;
      }
    }

    if(room2cAmount[i] < 1){ // need at least 1 room2c
      printf("ROOM2C forced in zone %d\n", i);
      placed = false;

      zoneStart++; zoneEnd--;
      for(int y = zoneStart; y <= zoneEnd; y++){
        for(int x = 3; x <= Map::MAP_WIDTH - 3; x++){
          if (mapTemp[x][y] == 1) {   // see if adding some rooms is possible
            if (mapTemp[x - 1][y] > 0) {
              if (mapTemp[x][y - 1] + mapTemp[x][y + 1] + mapTemp[x + 2][y] == 0) {
                if (mapTemp[x + 1][y - 2] + mapTemp[x + 2][y - 1] + mapTemp[x + 1][y - 1] == 0) {
                  mapTemp[x][y] = 2;
                  mapTemp[x + 1][y] = 2;
                  printf("ROOM2C forced in (%d, %d)\n", x+1, y);
                  mapTemp[x + 1][y - 1] = 1;
                  placed = true;
                } else if (mapTemp[x + 1][y + 2] + mapTemp[x + 2][y + 1] + mapTemp[x + 1][y + 1] == 0) {
                  mapTemp[x][y] = 2;
                  mapTemp[x + 1][y] = 2;
                  printf("ROOM2C forced into slot (%d, %d)\n", x+1, y);
                  mapTemp[x + 1][y + 1] = 1;
                  placed = true;
                }
              }
            } else if (mapTemp[x + 1][y] > 0) {
              if (mapTemp[x][y - 1] + mapTemp[x][y + 1] + mapTemp[x - 2][y] == 0) {
                if (mapTemp[x - 1][y - 2] + mapTemp[x - 2][y - 1] + mapTemp[x - 1][y - 1] == 0) {
                  mapTemp[x][y] = 2;
                  mapTemp[x - 1][y] = 2;
                  printf("ROOM2C at (%d, %d) forced!\n", (x-1), y);
                  mapTemp[x - 1][y - 1] = 1;
                  placed = true;
                } else if (mapTemp[x - 1][y + 2] + mapTemp[x - 2][y + 1] + mapTemp[x - 1][y + 1] == 0) {
                  mapTemp[x][y] = 2;
                  mapTemp[x - 1][y] = 2;
                  printf("ROOM2c at (%d, %d) foced!\n", (x-1), y);
                  mapTemp[x - 1][y + 1] = 1;
                  placed = true;
                }
              }
            } else if (mapTemp[x][y - 1] > 0) {
              if (mapTemp[x - 1][y] + mapTemp[x + 1][y] + mapTemp[x][y + 2] == 0) {
                if (mapTemp[x - 2][y + 1] + mapTemp[x - 1][y + 2] + mapTemp[x - 1][y + 1] == 0) {
                  mapTemp[x][y] = 2;
                  mapTemp[x][y + 1] = 2;
                  printf("ROOM2C forced into slot (%d, %d)\n", x, y+1);
                  mapTemp[x - 1][y + 1] = 1;
                  placed = true;
                } else if (mapTemp[x + 2][y + 1] + mapTemp[x + 1][y + 2] + mapTemp[x + 1][y + 1] == 0) {
                  mapTemp[x][y] = 2;
                  mapTemp[x][y + 1] = 2;
                  printf("ROOM2C forced into slot (%d, %d)\n", x, y+1);
                  mapTemp[x + 1][y + 1] = 1;
                  placed = true;
                }
              }
            } else if (mapTemp[x][y + 1] > 0) {
              if (mapTemp[x - 1][y] + mapTemp[x + 1][y] + mapTemp[x][y - 2] == 0) {
                if (mapTemp[x - 2][y - 1] + mapTemp[x - 1][y - 2] + mapTemp[x - 1][y - 1] == 0) {
                  mapTemp[x][y] = 2;
                  mapTemp[x][y - 1] = 2;
                  printf("ROOM2C forced into slot (%d, %d)\n", x, y - 1);
                  mapTemp[x - 1][y - 1] = 1;
                  placed = true;
                } else if (mapTemp[x + 2][y - 1] + mapTemp[x + 1][y - 2] + mapTemp[x + 1][y - 1] == 0) {
                  mapTemp[x][y] = 2;
                  mapTemp[x][y - 1] = 2;
                  printf("ROOM2C forced into slot (%d, %d)\n", x, y-1);
                  mapTemp[x + 1][y - 1] = 1;
                  placed = true;
                }
              }
            }
            if (placed) {
                room2cAmount[i]++;
                room2Amount[i]++;
            }
          }

          if(placed) break;
        }
        if(placed) break;
      }
    }
  }
}