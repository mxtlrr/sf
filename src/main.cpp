#include <iostream>

#include "backend/find.hpp"
#include "backend/map.hpp"
#include "util.hpp"

int main(void){
  Room room1; room1.x = 120.000; room1.z = 128.000; room1.rt.name = "914";
  Room room2; room2.x = 112.000; room1.z = 112.000; room1.rt.name = "room2sl";
  std::vector<Room> pattern;
  pattern.push_back(room1); pattern.push_back(room2);
  std::vector<SeedResults> results = patternFind(pattern);

  // do something...

  return 0;
}