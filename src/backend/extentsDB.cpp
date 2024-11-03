#include "backend/extentsDB.hpp"


std::unordered_map<std::string, std::unique_ptr<RoomExtentsDB>> RoomExtentsDB::DB;
RoomExtentsDB::RoomExtentsDB(){
  RMeshReader r("roomdb.dat");
  DB.clear();

  while(!r.isEof()){
    std::string name = r.readString();
    for(int a = 0; a <= 630; a += 90){
      int angle = r.readInt();
      auto roomDb = std::make_unique<RoomExtentsDB>();

      for(int i = 1; i < 18; i++){
        Bounds b;
        b.minX = r.readFloat();
        b.maxX = r.readFloat();
        b.minZ = r.readFloat();
        b.maxZ = r.readFloat();
        roomDb->bounds[i] = b;
      }

      DB[name + std::to_string(angle)] = std::move(roomDb);
    }
  }
}

RoomExtentsDB::Bounds RoomExtentsDB::findExtents(const std::string name,
      int angle, int x, int z){
  Bounds b;
  auto key = name + std::to_string(angle);
  auto it = DB.find(key);
  if(it != DB.end()){
    const RoomExtentsDB* db = it->second.get();
    b.minX = db->bounds.at(x).minX;
    b.maxX = db->bounds.at(x).maxX;
    b.minZ = db->bounds.at(z).minZ;
    b.maxZ = db->bounds.at(z).maxZ;
  }

  return b;
}