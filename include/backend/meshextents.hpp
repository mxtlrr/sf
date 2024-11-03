#ifndef __MESH_EXTENTS_HPP_
#define __MESH_EXTENTS_HPP_

#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

class MeshExtents {
public:
    double minX, minY, minZ;
    double maxX, maxY, maxZ;

    MeshExtents(double minX, double minY, double minZ, double maxX, double maxY, double maxZ)
        : minX(minX), minY(minY), minZ(minZ), maxX(maxX), maxY(maxY), maxZ(maxZ) {}

    MeshExtents& rotate(int angle);
    MeshExtents& scale(double scale);
    MeshExtents copyTransform(double scale, int angle);
    std::string toString();
};

#endif