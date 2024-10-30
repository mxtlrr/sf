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
  std::vector<std::vector<std::string>> mapRoom(
    ROOM4 + 1,
    std::vector<std::string>(room2Amount[0] + room2Amount[1] + room2Amount[2] + 3)
  );

  defineRooms();
  // createRooms();
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

///////////////////////////////////////// room generation
void Map::defineRooms(){
  int minPos = 1;
  int maxPos = room1Amount[0] - 1;

  printf("minPos = %d | maxPos = %d\n", minPos, maxPos);


  // generating out of range... weird...
  mapRoom.at(ROOM1).at(0) = "start";
  
  
  return;



  setRoom("roompj", ROOM1, (int)(floor(0.1 * room1Amount[0])), minPos, maxPos);
  setRoom("914", ROOM1, (int)(floor(0.3 * room1Amount[0])), minPos, maxPos);
  setRoom("room1archive", ROOM1, (int)(floor(0.5 * room1Amount[0])), minPos, maxPos);
  setRoom("room205", ROOM1, (int)(floor(0.6 * room1Amount[0])), minPos, maxPos);

  mapRoom[ROOM2C][0] = "lockroom";

  maxPos = room2Amount[0] - 1;

  mapRoom[ROOM2][0] = "room2closets";
  setRoom("room2testroom2", ROOM2, (int)(floor(0.1 * room2Amount[0])), minPos, maxPos);
  setRoom("room2scps", ROOM2, (int)(floor(0.2 * room2Amount[0])), minPos, maxPos);
  setRoom("room2storage", ROOM2, (int)(floor(0.3 * room2Amount[0])), minPos, maxPos);
  setRoom("room2gw_b", ROOM2, (int)(floor(0.4 * room2Amount[0])), minPos, maxPos);
  setRoom("room2sl", ROOM2, (int)(floor(0.5 * room2Amount[0])), minPos, maxPos);
  setRoom("room012", ROOM2, (int)(floor(0.55 * room2Amount[0])), minPos, maxPos);
  setRoom("room2scps2", ROOM2, (int)(floor(0.6 * room2Amount[0])), minPos, maxPos);
  setRoom("room1123", ROOM2, (int)(floor(0.7 * room2Amount[0])), minPos, maxPos);
  setRoom("room2elevator", ROOM2, (int)(floor(0.85 * room2Amount[0])), minPos, maxPos);

  mapRoom[ROOM3][(int)floor(bbRnd(0.2f, 0.8f) * room3Amount[0])] = "room3storage";

  mapRoom[ROOM2C][(int)floor(0.5 * room2cAmount[0])] = "room1162";

  mapRoom[ROOM4][(int)floor(0.3 * room4Amount[0])] = "room4info";

  // zone 2
  minPos = room1Amount[0];
  maxPos = minPos + room1Amount[1] - 1;

  setRoom("room079", ROOM1, room1Amount[0] + (int)(floor(0.15 * room1Amount[1])), minPos, maxPos);
  setRoom("room106", ROOM1, room1Amount[0] + (int)(floor(0.3 * room1Amount[1])), minPos, maxPos);
  setRoom("008", ROOM1, room1Amount[0] + (int)(floor(0.4 * room1Amount[1])), minPos, maxPos);
  setRoom("room035", ROOM1, room1Amount[0] + (int)(floor(0.5 * room1Amount[1])), minPos, maxPos);
  setRoom("coffin", ROOM1, room1Amount[0] + (int)(floor(0.7 * room1Amount[1])), minPos, maxPos);

  minPos = room2Amount[0];
  maxPos = minPos + room2Amount[1] - 1;

  mapRoom[ROOM2][minPos + (int)(floor(0.1 * room2Amount[1]))] = "room2nuke";
  setRoom("room2tunnel", ROOM2, minPos + (int)(floor(0.25 * room2Amount[1])), minPos, maxPos);
  setRoom("room049", ROOM2, minPos + (int)(floor(0.4 * room2Amount[1])), minPos, maxPos);
  setRoom("room2shaft", ROOM2, minPos + (int)(floor(0.6 * room2Amount[1])), minPos, maxPos);
  setRoom("testroom", ROOM2, minPos + (int)(floor(0.7 * room2Amount[1])), minPos, maxPos);
  setRoom("room2servers", ROOM2, minPos + (int)(floor(0.9 * room2Amount[1])), minPos, maxPos);

  mapRoom[ROOM3][room3Amount[0] + (int)floor(0.3 * room3Amount[1])] = "room513";
  mapRoom[ROOM3][room3Amount[0] + (int)floor(0.6 * room3Amount[1])] = "room966";

  mapRoom[ROOM2C][room2cAmount[0] + (int)floor(0.5 * room2cAmount[1])] = "room2cpit";

  // zone 3
  mapRoom[ROOM1][room1Amount[0] + room1Amount[1] + room1Amount[2] - 2] = "exit1";
  mapRoom[ROOM1][room1Amount[0] + room1Amount[1] + room1Amount[2] - 1] = "gateaentrance";
  mapRoom[ROOM1][room1Amount[0] + room1Amount[1]] = "room1lifts";

  minPos = room2Amount[0] + room2Amount[1];
  maxPos = minPos + room2Amount[2] - 1;

  mapRoom[ROOM2][minPos + (int)(floor(0.1 * room2Amount[2]))] = "room2poffices";
  setRoom("room2cafeteria", ROOM2, minPos + (int)(floor(0.2 * room2Amount[2])), minPos, maxPos);
  setRoom("room2sroom", ROOM2, minPos + (int)(floor(0.3 * room2Amount[2])), minPos, maxPos);
  setRoom("room2servers2", ROOM2, minPos + (int)(floor(0.4 * room2Amount[2])), minPos, maxPos);
  setRoom("room2offices", ROOM2, minPos + (int)(floor(0.45 * room2Amount[2])), minPos, maxPos);
  setRoom("room2offices4", ROOM2, minPos + (int)(floor(0.5 * room2Amount[2])), minPos, maxPos);
  setRoom("room860", ROOM2, minPos + (int)(floor(0.6 * room2Amount[2])), minPos, maxPos);
  setRoom("medibay", ROOM2, minPos + (int)(floor(0.7 * room2Amount[2])), minPos, maxPos);
  setRoom("room2poffices2", ROOM2, minPos + (int)(floor(0.8 * room2Amount[2])), minPos, maxPos);
  setRoom("room2offices2", ROOM2, minPos + (int)(floor(0.9 * room2Amount[2])), minPos, maxPos);

  int r2c = room2cAmount[0] + room2cAmount[1];
  mapRoom[ROOM2C][r2c] = "room2ccont";
  mapRoom[ROOM2C][r2c + 1] = "lockroom2";

  int r3 = room3Amount[0] + room3Amount[1];
  mapRoom[ROOM3][r3 + (int)(floor(0.3 * room3Amount[2]))] = "room3servers";
  mapRoom[ROOM3][r3 + (int)(floor(0.7 * room3Amount[2]))] = "room3servers2";
  mapRoom[ROOM3][r3 + (int)(floor(0.5 * room3Amount[2]))] = "room3offices";
}

void Map::setRoom(std::string roomName, int type, int pos, int min, int max){
  std::cout << "setRoom() call. name: " << roomName << "\n";
  bool looped = false;
  bool canPlace = true;
  while(Map::mapRoom[type][pos] != ""){
    pos++;
    if(pos > max){
      if(!looped){
        pos = min + 1;
        looped = true;
      } else {
        canPlace = false;
        break;
      }
    }
  }

  printf("%s // pos: %.3d...", roomName.c_str(), pos);
  if(canPlace){
    Map::mapRoom[type][pos] = roomName;
    printf("placed!\n");
  } else {
    printf("failed to be placed!\n");
  }
}