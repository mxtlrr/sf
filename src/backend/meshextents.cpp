#include "backend/meshextents.hpp"

MeshExtents& MeshExtents::rotate(int angle) {
  angle %= 360;
  double rads = angle * M_PI / 180.0;

  double xr = minX * std::cos(rads) - minZ * std::sin(rads);
  double zr = minX * std::sin(rads) + minZ * std::cos(rads);
  minX = xr;
  minZ = zr;

  xr = maxX * std::cos(rads) - maxZ * std::sin(rads);
  zr = maxX * std::sin(rads) + maxZ * std::cos(rads);
  maxX = xr;
  maxZ = zr;

  // Fix inverted Min/Max values
  if (minX > maxX) std::swap(minX, maxX);
  if (minZ > maxZ) std::swap(minZ, maxZ);

  return *this;
}

MeshExtents& MeshExtents::scale(double scale) {
  minX *= scale;
  minY *= scale;
  minZ *= scale;
  maxX *= scale;
  maxY *= scale;
  maxZ *= scale;

  return *this;
}

MeshExtents MeshExtents::copyTransform(double scale, int angle) {
  return MeshExtents(minX, minY, minZ, maxX, maxY, maxZ).scale(scale).rotate(angle);
}

std::string MeshExtents::toString() {
  std::ostringstream ss;
  ss << std::fixed << std::setprecision(2) 
    << minX << ", " << minY << ", " << minZ << " / "
    << maxX << ", " << maxY << ", " << maxZ;
  return ss.str();
}