#include "backend/find.hpp"

std::vector<SeedResults> patternFind(std::vector<Room> pattern){
  std::vector<SeedResults> res(LIMIT);

  for(unsigned int i = 1; i <= LIMIT; i++){
    SeedResults r;    // Current results
    unsigned short roomsFound = 0; // How many rooms have we found on this
                                   // seed?
    printf("Creating map for seed=%d\n", i);
    Map::createMap(i); r.seedId = i; // Create the seed
    removeDuplicates();
    unsigned short roomCount = (unsigned short)(Map::rooms.size());

    // Gross code, but I think it should work.
    for(auto& rooms : Map::rooms){
      for(auto& room : pattern){
        // Angle doesn't really need to be checked.
        // TODO: check zone? won't really matter though. we'll see
        if(room.x == rooms.x && room.z == rooms.z
            && room.rt.name == rooms.rt.name)
          roomsFound++;
      }
    }

    printf("Checked seed %d\n", i);

    clearVector();      // Prep for next
    r.accuracy = (float)(roomsFound / roomCount);
    res.push_back(r);
  }

  return res;
}