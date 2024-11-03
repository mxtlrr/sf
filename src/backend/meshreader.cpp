#include "backend/meshreader.hpp"

RMeshReader::RMeshReader(std::string filename){
  FILE* fp = fopen(filename.c_str(), "rb");
  if(!fp){
    printf("[!] Can't read %s\n", filename.c_str());
    exit(1);
  }

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  content = (unsigned char*)malloc(size);
  if(!content){
    printf("Can't malloc() %ld bytes for reading\n", size);
    exit(1);
  }

  fread(content, size, 1, fp);
  fclose(fp);
}

std::string RMeshReader::readString(){
  int length = readInt();
  std::string result(reinterpret_cast<const char*>(content + caret), length);
  caret += length;
  return result;
}

int RMeshReader::readInt(){
  int length = 0;
  for (int i = 3; i >= 0; i--)
    length = (length << 8) + (content[caret + i] & 0xFF);
  caret += 4;
  return length;
}

unsigned char RMeshReader::readByte(){
  return content[caret++];
}

float RMeshReader::readFloat(){
  // Note the error here only occurs if you're not using C++20 and higher.
  // Ignore this error in VSCode / your linter.
  return std::bit_cast<float>(readInt());
}

bool RMeshReader::isEof() {
  return caret>=size;
}

unsigned char* RMeshReader::getContent() {
    return content;
}