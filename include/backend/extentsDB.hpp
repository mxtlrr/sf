#ifndef __EXTENTS_DB_HPP_
#define __EXTENTS_DB_HPP_

#include <unordered_map>
#include <iostream>
#include <string>
#include <memory>

#include "backend/meshreader.hpp"

class RoomExtentsDB {
  public:
    struct Bounds {
      float minX;
      float maxX;
      float minZ;
      float maxZ;
    };

    RoomExtentsDB();
    static Bounds findExtents(const std::string name,
          int angle, int x, int z);

  private:
    static std::unordered_map<std::string, std::unique_ptr<RoomExtentsDB>> DB;
    std::unordered_map<int, Bounds> bounds;
};

#endif