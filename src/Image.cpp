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

void Image::encode(const String& filePath)
{
  std::fstream file(filePath, std::ios::binary | std::ios::out);

  if (!file.is_open())
  {
    return;
  }

  BITMAP_HEADER header{0};

  header.fileHeader.signature = BMP_TYPE;
  header.fileHeader.fileSize = (m_height * getPitch()) + sizeof(BITMAP_HEADER);
  header.fileHeader.reserved1 = 0;
  header.fileHeader.reserved2 = 0;
  header.fileHeader.offsetToData = sizeof(BITMAP_HEADER);

  header.dibHeader.size = sizeof(BITMAP_DIB_HEADER);
  header.dibHeader.width = m_width;
  header.dibHeader.height = m_height;
  header.dibHeader.planes = 1;
  header.dibHeader.bitsPerPixel = m_bpp;
  header.dibHeader.compression = 0;
  header.dibHeader.imageSize = m_width * m_height;
  header.dibHeader.xPixelsPerMeter = 0;
  header.dibHeader.yPixelsPerMeter = 0;
  header.dibHeader.colorsUsed = 0;
  header.dibHeader.importantColors = 0;

  file.write(reinterpret_cast<char*>(&header), sizeof(BITMAP_HEADER));

  int padding = getPitch() % 4;
  int lineMemoryWidth = getPitch();

  if (padding)
  {
    padding = 4 - padding;
    lineMemoryWidth += padding;
  }

  char paddBuffer[3] = { 0,0,0 };

  for (int line = m_height - 1; line >= 0; --line)
  {

    file.write(reinterpret_cast<char*>(&m_pixels[getPitch() * line]), getPitch());
    if (padding)
    {
      file.write(paddBuffer, padding);
    }
  }


  file.close();
}

void Image::decode(const String& filePath)
{
  
  std::fstream file(filePath, std::ios::binary | std::ios::in);

  if (!file.is_open()) {
    return;
  }
  
  BITMAP_FILE_HEADER fileHeader {0};
  BITMAP_DIB_HEADER dibHeader   {0};

  file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAP_FILE_HEADER));

  if (fileHeader.signature != BMP_TYPE)
  {
    return;
  }

  file.read(reinterpret_cast<char*>(&dibHeader), sizeof(BITMAP_DIB_HEADER));

  
  file.seekg(fileHeader.offsetToData);

  create(dibHeader.width,dibHeader.height,dibHeader.bitsPerPixel);

  //Continues

  int padding = getPitch() % 4;
  int lineMemoryWidth = getPitch();

  if (padding)
  {
    padding = 4 - padding;
    lineMemoryWidth += padding;
  }


  for (int line = m_height - 1; line >= 0; --line)
  {
    file.seekp(lineMemoryWidth * line + fileHeader.offsetToData);
    file.read(reinterpret_cast<char*>(&m_pixels[getPitch() * (m_height - 1 - line)]), getPitch());
  }
  
  file.close();
}
