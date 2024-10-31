#include "backend/map.hpp"

int ROOM1  = 1;
int ROOM2  = 2;
int ROOM2C = 3;
int ROOM3  = 4;
int ROOM4  = 5;
namespace Map {

  int mapTemp[MAP_WIDTH+1][MAP_HEIGHT+1];
  int room1Amount[3];
  int room2Amount[3];
  int room2cAmount[3];
  int room3Amount[3];
  int room4Amount[3];

  std::vector<std::vector<std::string>> mapRoom;
};


void Map::createMap(int seed){
  bbSeedRnd(seed);
  countRooms();

  enrichRoom1s();
  enrichRoom2c4s();

  // Generate bounds for both dimensions.
  mapRoom = std::vector<std::vector<std::string>>(
    ROOM4 + 1,
    std::vector<std::string>(room2Amount[0] + room2Amount[1] + room2Amount[2] + 3)
);

  defineRooms();
  // createRooms();
}

void Map::countRooms(){
        int zone;
        // count the amount of rooms
        for (int y = 1; y <= Map::MAP_HEIGHT; y++) {
            zone = getZone(y);

            for (int x = 1; x <= Map::MAP_WIDTH - 1; x++) {
                if (mapTemp[x][y] > 0) {
                    if (mapTemp[x][y] < 255)
                        mapTemp[x][y] = getConnections(mapTemp, x, y);
                    switch (mapTemp[x][y]) {
                        case 1:
                            room1Amount[zone]++;
                            break;
                        case 2:
                            if (getHorizontalConnections(mapTemp, x, y) == 2)
                                room2Amount[zone]++;
                            else if (getVerticalConnections(mapTemp, x, y) == 2)
                                room2Amount[zone]++;
                            else
                                room2cAmount[zone]++;
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
  printf("enrichRoom1s() call\n");
  int x2 = 0, y2 = 0, roomsLeft;

  // force more room1s (if needed) for each zone
  for (int i = 0; i <= 2; i++) {
    // need more rooms if there are less than 5 of them
    roomsLeft = 5 - room1Amount[i];
    if (roomsLeft > 0) {
      for (int y = (Map::MAP_HEIGHT / ZONE_AMOUNT) * (2 - i) + 1; y <= ((Map::MAP_HEIGHT / ZONE_AMOUNT) * ((2 - i) + 1)) - 2; y++) {
        for (int x = 2; x <= Map::MAP_WIDTH - 2; x++) {
          if (mapTemp[x][y] == 0) {
            if (getConnections(mapTemp, x, y) == 1) {
                if (mapTemp[x + 1][y] != 0) {
                    x2 = x + 1;
                    y2 = y;
                } else if (mapTemp[x - 1][y] != 0) {
                    x2 = x - 1;
                    y2 = y;
                } else if (mapTemp[x][y + 1] != 0) {
                    x2 = x;
                    y2 = y + 1;
                } else if (mapTemp[x][y - 1] != 0) {
                    x2 = x;
                    y2 = y - 1;
                }

                bool placed = false;
                if (mapTemp[x2][y2] > 1 && mapTemp[x2][y2] < 4) {
                    switch (mapTemp[x2][y2]) {
                        case 2:
                            if (getHorizontalConnections(mapTemp, x2, y2) == 2) {
                                room2Amount[i]--;
                                room3Amount[i]++;
                                placed = true;
                            } else if (getVerticalConnections(mapTemp, x2, y2) == 2) {
                                room2Amount[i]--;
                                room3Amount[i]++;
                                placed = true;
                            }
                            break;
                        case 3:
                            room3Amount[i]--;
                            room4Amount[i]++;
                            placed = true;
                    }

                      if (placed) {
                          mapTemp[x2][y2]++;

                          mapTemp[x][y] = 1;
                          room1Amount[i]++;

                          roomsLeft--;
                      }
                  }
              }
          }
          if (roomsLeft == 0)
            break;
        }
        if (roomsLeft == 0)
          break;
      }
    }
  }
}

void Map::enrichRoom2c4s(){
        // force more room4s and room2Cs
        int zoneStart = 0, zoneEnd = 0;
        bool placed;
        for (int i = 0; i <= 2; i++) {

            switch (i) {
                case 2:
                    zoneStart = 2;
                    zoneEnd = Map::MAP_HEIGHT / 3;
                    break;
                case 1:
                    zoneStart = Map::MAP_HEIGHT / 3 + 1;
                    zoneEnd = (int) (Map::MAP_HEIGHT * (2.0 / 3.0) - 1);
                    break;
                case 0:
                    zoneStart = (int) (Map::MAP_HEIGHT * (2.0 / 3.0) + 1);
                    zoneEnd = Map::MAP_HEIGHT - 2;
            }

            if (room4Amount[i] < 1) {   // we want at least 1 ROOM4
                //System.out.println("forcing a ROOM4 into zone " + i);
                placed = false;

                for (int y = zoneStart; y <= zoneEnd; y++) {
                    for (int x = 2; x <= Map::MAP_WIDTH - 2; x++) {
                        if (mapTemp[x][y] == 3) {   // see if adding a ROOM1 is possible
                            if (mapTemp[x + 1][y] == 0 && mapTemp[x + 1][y + 1] == 0 && mapTemp[x + 1][y - 1] == 0 && mapTemp[x + 2][y] == 0) {
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
                            if (placed) {
                                mapTemp[x][y] = 4; // turn this room into a ROOM4
                                //System.out.println("ROOM4 forced into slot (" + x + ", " + y + ")");
                                room4Amount[i]++;
                                room3Amount[i]--;
                                room1Amount[i]++;
                            }
                        }
                        if (placed)
                            break;
                    }
                    if (placed)
                        break;
                }
            }

            if (room2cAmount[i] < 1) {  // we want at least 1 ROOM2C
                //System.out.println("forcing a ROOM2C into zone " + i);
                placed = false;

                zoneStart++;
                zoneEnd--;

                for (int y = zoneStart; y <= zoneEnd; y++) {
                    for (int x = 3; x <= Map::MAP_WIDTH - 3; x++) {
                        if (mapTemp[x][y] == 1) {   // see if adding some rooms is possible
                            if (mapTemp[x - 1][y] > 0) {
                                if (mapTemp[x][y - 1] + mapTemp[x][y + 1] + mapTemp[x + 2][y] == 0) {
                                    if (mapTemp[x + 1][y - 2] + mapTemp[x + 2][y - 1] + mapTemp[x + 1][y - 1] == 0) {
                                        mapTemp[x][y] = 2;
                                        mapTemp[x + 1][y] = 2;
                                        //System.out.println("ROOM2C forced into slot (" + (x + 1) + ", " + (y) + ")");
                                        mapTemp[x + 1][y - 1] = 1;
                                        placed = true;
                                    } else if (mapTemp[x + 1][y + 2] + mapTemp[x + 2][y + 1] + mapTemp[x + 1][y + 1] == 0) {
                                        mapTemp[x][y] = 2;
                                        mapTemp[x + 1][y] = 2;
                                        //System.out.println("ROOM2C forced into slot (" + (x + 1) + ", " + (y) + ")");
                                        mapTemp[x + 1][y + 1] = 1;
                                        placed = true;
                                    }
                                }
                            } else if (mapTemp[x + 1][y] > 0) {
                                if (mapTemp[x][y - 1] + mapTemp[x][y + 1] + mapTemp[x - 2][y] == 0) {
                                    if (mapTemp[x - 1][y - 2] + mapTemp[x - 2][y - 1] + mapTemp[x - 1][y - 1] == 0) {
                                        mapTemp[x][y] = 2;
                                        mapTemp[x - 1][y] = 2;
                                        //System.out.println("ROOM2C forced into slot (" + (x - 1) + ", " + (y) + ")");
                                        mapTemp[x - 1][y - 1] = 1;
                                        placed = true;
                                    } else if (mapTemp[x - 1][y + 2] + mapTemp[x - 2][y + 1] + mapTemp[x - 1][y + 1] == 0) {
                                        mapTemp[x][y] = 2;
                                        mapTemp[x - 1][y] = 2;
                                        //System.out.println("ROOM2C forced into slot (" + (x - 1) + ", " + (y) + ")");
                                        mapTemp[x - 1][y + 1] = 1;
                                        placed = true;
                                    }
                                }
                            } else if (mapTemp[x][y - 1] > 0) {
                                if (mapTemp[x - 1][y] + mapTemp[x + 1][y] + mapTemp[x][y + 2] == 0) {
                                    if (mapTemp[x - 2][y + 1] + mapTemp[x - 1][y + 2] + mapTemp[x - 1][y + 1] == 0) {
                                        mapTemp[x][y] = 2;
                                        mapTemp[x][y + 1] = 2;
                                        //System.out.println("ROOM2C forced into slot (" + x + ", " + (y + 1) + ")");
                                        mapTemp[x - 1][y + 1] = 1;
                                        placed = true;
                                    } else if (mapTemp[x + 2][y + 1] + mapTemp[x + 1][y + 2] + mapTemp[x + 1][y + 1] == 0) {
                                        mapTemp[x][y] = 2;
                                        mapTemp[x][y + 1] = 2;
                                        //System.out.println("ROOM2C forced into slot (" + (x) + ", " + (y + 1) + ")");
                                        mapTemp[x + 1][y + 1] = 1;
                                        placed = true;
                                    }
                                }
                            } else if (mapTemp[x][y + 1] > 0) {
                                if (mapTemp[x - 1][y] + mapTemp[x + 1][y] + mapTemp[x][y - 2] == 0) {
                                    if (mapTemp[x - 2][y - 1] + mapTemp[x - 1][y - 2] + mapTemp[x - 1][y - 1] == 0) {
                                        mapTemp[x][y] = 2;
                                        mapTemp[x][y - 1] = 2;
                                        //System.out.println("ROOM2C forced into slot (" + x + ", " + (y - 1) + ")");
                                        mapTemp[x - 1][y - 1] = 1;
                                        placed = true;
                                    } else if (mapTemp[x + 2][y - 1] + mapTemp[x + 1][y - 2] + mapTemp[x + 1][y - 1] == 0) {
                                        mapTemp[x][y] = 2;
                                        mapTemp[x][y - 1] = 2;
                                        //System.out.println("ROOM2C forced into slot (" + (x) + ", " + (y - 1) + ")");
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
                        if (placed)
                            break;
                    }
                    if (placed)
                        break;
                }
            }
        }
}

///////////////////////////////////////// room generation
void Map::defineRooms(){
  // zone 1
  int minPos = 1;
  int maxPos = room1Amount[0] - 1;

  mapRoom[ROOM1][0] = "start";
  setRoom("roompj", ROOM1, (int) (floor(0.1 * room1Amount[0])), minPos, maxPos);
  setRoom("914", ROOM1, (int) (floor(0.3 * room1Amount[0])), minPos, maxPos);
  setRoom("room1archive", ROOM1, (int) (floor(0.5 * room1Amount[0])), minPos, maxPos);
  setRoom("room205", ROOM1, (int) (floor(0.6 * room1Amount[0])), minPos, maxPos);

  mapRoom[ROOM2C][0] = "lockroom";

  maxPos = room2Amount[0] - 1;

  mapRoom[ROOM2][0] = "room2closets";
  setRoom("room2testroom2", ROOM2, (int) (floor(0.1 * room2Amount[0])), minPos, maxPos);
  setRoom("room2scps", ROOM2, (int) (floor(0.2 * room2Amount[0])), minPos, maxPos);
  setRoom("room2storage", ROOM2, (int) (floor(0.3 * room2Amount[0])), minPos, maxPos);
  setRoom("room2gw_b", ROOM2, (int) (floor(0.4 * room2Amount[0])), minPos, maxPos);
  setRoom("room2sl", ROOM2, (int) (floor(0.5 * room2Amount[0])), minPos, maxPos);
  setRoom("room012", ROOM2, (int) (floor(0.55 * room2Amount[0])), minPos, maxPos);
  setRoom("room2scps2", ROOM2, (int) (floor(0.6 * room2Amount[0])), minPos, maxPos);
  setRoom("room1123", ROOM2, (int) (floor(0.7 * room2Amount[0])), minPos, maxPos);
  setRoom("room2elevator", ROOM2, (int) (floor(0.85 * room2Amount[0])), minPos, maxPos);

  mapRoom[ROOM3][(int) floor(bbRnd(0.2f, 0.8f) * room3Amount[0])] = "room3storage";
  mapRoom[ROOM2C][(int) floor(0.5 * room2cAmount[0])] = "room1162";
  mapRoom[ROOM4][(int)floor(0.3 * room4Amount[0])] = "room4info";

  printf("zone 1 done!\n");

  // TODO: generate zone 2 and 3...
  // currently they segfault (bad)
}

void Map::setRoom(std::string roomName, int type, int pos, int min, int max){
  bool looped = false;
  bool canPlace = true;
  while (mapRoom[type][pos] != "") {
    pos++;
    if (pos > max) {
      if (!looped) {
        pos = min + 1;
        looped = true;
      } else {
        canPlace = false;
        break;
      }
    }
  }
  if (canPlace) {
    printf("could place     %s\n", roomName.c_str());
    mapRoom[type][pos] = roomName;
  } else
    printf("couldn't place %s\n", roomName.c_str());
}