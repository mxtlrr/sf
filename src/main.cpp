#include "backend/map.hpp"

int main(void){
  Map::createMap(648);

  int count = 1;
  for(auto& vec : Map::mapRoom){
    printf("This is vector %d\n", count);
    for(auto& elem : vec){
      std::cout << "\tElement: " << elem << std::endl;
    }
    count++;
  }
  printf("%s\n", Map::mapRoom[0][0].c_str());
  return 0;
}