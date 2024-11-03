#ifndef __MESHREADER_HPP_
#define __MESHREADER_HPP_

#include <iostream>
#include <bit>
#include <cstdint>

class RMeshReader {
  private:
    int caret = 0;
    unsigned char* content; 
    long int size;
  
  public:
    RMeshReader(std::string filename);
    std::string readString();
    int readInt();
    unsigned char readByte();
    float readFloat();
    bool isEof();
    unsigned char* getContent();
};

#endif