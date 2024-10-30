#include "backend/map.hpp"

int main(void){
  Map::createMap(648);

  for(const auto& row : Map::mapRoom){
    for(const auto& elem : row){
      std::cout << "element: " << elem << std::endl;
    }
  }
  return 0;
}