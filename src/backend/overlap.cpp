#include "backend/overlap.hpp"

bool checkOverlap(Room r1, Room r2){
  if(r1.maxX <= r2.minX || r1.maxZ <= r2.minZ) return false;
  if(r1.minX >= r2.maxX || r1.minZ >= r2.maxZ) return false;
  return true;
}