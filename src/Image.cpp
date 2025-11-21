#include "Image.h"
#include <fstream>


void Image::create(uint32 width,
                   uint32 height,
                   uint16 bitsPerPixel)
{
  m_width = width;
  m_height = height;
  m_bpp = bitsPerPixel;
  
  m_pixels = new uint8[height * getPitch()];
}

void Image::encode()
{

}

void Image::decode(const String& filePath)
{
  
  std::fstream file(filePath,std::ios::binary | std::ios::ate);
  
  if (!file.is_open()) {
    return;
  }
  
  auto fileSize = file.tellg(); //size of the file
  
  file.seekp(std::ios::beg);
  
  BITMAP_HEADER header{0};
  
  file.read(reinterpret_cast<char*>(&m_pixels), sizeof(BITMAP_HEADER));
  
  printf("The signature is %c",header.fileHeader.signature);
  
  
  
  
  
  
  
  file.close();
}
