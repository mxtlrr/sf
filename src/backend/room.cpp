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

void Room::calcExtents(){
  if(rt.disableOverlapCheck) return;

  extentsAngle = angle;
  extents = rt.extents.copyTransform((double)(8/2048), angle);

  if(x>0 && z>0){
    RoomExtentsDB::Bounds b = RoomExtentsDB::findExtents(rt.name,
          angle, (int)(x/8), (int)(z/8));
    minX = b.minX;
    maxX = b.maxX;
    minZ = b.minZ;
    maxZ = b.maxZ;
  } else {
    minX = extents.minX + 0.05 + x;
    minZ = extents.minZ + 0.05 + z;
    maxX = extents.maxX - 0.05 + x;
    maxZ = extents.maxZ - 0.05 + z;

    // funny bug frmo the game itself. lol
    if (minX > maxX) {
      double a = minX;
      minX = maxX;
      maxX = a;
    }
    if (minZ > maxZ) {
      double a = minZ;
      minZ = maxZ;
      maxZ = a;
    }
    printf("Room %s extents: %f,%f,%f / %f,%f,%f / %s deg\n",
        rt.name.c_str(), minX, minY, minZ, maxX, maxY, maxZ);
  }
}