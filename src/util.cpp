#include "util.hpp"

void removeDuplicates(){
  printf("vector size -> %ld\n", Map::rooms.size());
  std::vector<double> ex;
  int z = 0;
  for(auto& v : Map::rooms){
    if(find(ex.begin(), ex.end(), v.x) != ex.end()){
      // Doesn't exist, add it
      ex.push_back(v.x);
    } else {
      // Already exists -- probably a dupe, delete it.
      std::set<Room> s;
      for(Room i : Map::rooms)
        s.insert(i);
      Map::rooms.assign(s.begin(), s.end());
    }
    z++;
  }
  printf("new size of vector (duplicates removed): %ld\n", Map::rooms.size());
}