#include "backend/room.hpp"

Room Room::createRoom(int zone, int shape, int x, int z, std::string name) {
  Room r;// = {
  r.shape = shape;
  r.zone = zone;

  r.x = x;
  r.z = z;

  /* Not sure needed if this is needed... we just need the coordinates
   * of each room, and the name
  if(!(name.empty()) && name != nullptr && name.length() > 0){
    // Look up template in finder... TODO?
  } */
  // Template has name
  r.rt.name = name;
  return r;
}

bool validRoom(Room r){
  return !(r.rt.name.empty());
  // if(r.rt.name.empty() == false){
    // return true;
  // }
  // return false;
}

bool existRooms(std::vector<Room> r, Room n){
  for(auto& v : r){
    // Only compare zone, shape, x and z.
    if(v.x == n.x || v.z == n.z ||
      v.shape == n.shape || v.zone == n.zone)
      return false; // It's the same!
  }
  return true;  // No match:)
}