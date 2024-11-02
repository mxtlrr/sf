#ifndef __ROOM_HPP_
#define __ROOM_HPP_

#include <iostream>
#include <vector>


// Lotta this stuff I don't need tbh
class RoomTemplate {
  private:
    int id;
    int large;
    int useLightCones;
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
  
  public:
    std::string name;
    int shape;
    int zone[5];
    int commonness;
    bool disableDecals;
    bool disableOverlapCheck;
};

class Room {
  public:
    int zone;
    double x, z;
    int angle, shape;

    // template?
    RoomTemplate rt;

    static Room createRoom(int zone, int shape, int x, int z, std::string name);

    // find() and some std::set things like std::set.insert
    // require these operators. TODO: move to other file
    bool operator<(const Room& other) const {
      if(x != other.x) return (x<other.x);
      if(z != other.z) return (z<other.z);
      return false; // x = z, x<z not true.
    }

    bool operator==(const Room& other) const {
      return x == other.x && z == other.z;
    }
};


// Is r a valid room? Checks:
//  r.zone   > 0
//  r.shape  > 0
//  r.x     >= 0, r.x < 500(?) (exception is gate A)
//  r.z     >= 0
//  r.name.empty() = false.
bool validRoom(Room r);

// Do not add duplicate rooms to the vector. This will skew results
// greatly. Parameters:
// --> r: vector of rooms
// --> n: room we're checking 
bool existRooms(std::vector<Room> r, Room n);

#endif