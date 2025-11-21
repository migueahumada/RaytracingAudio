#pragma once
#include "Prerequisites.h"

#define BMP_TYPE 'MB'

#pragma pack(push, 2)
struct BITMAP_FILE_HEADER
{
  uint16 signature;
  uint32 fileSize;
  uint16 reserved1;
  uint16 reserved2;
  uint32 offsetToData;
};


struct BITMAP_DIB_HEADER
{
  uint32 size;
  uint32 width;
  uint32 height;
  uint16 planes;
  uint16 bitsPerPixel;
  uint32 compression;
  uint32 imageSize;
  uint32 xPixelsPerMeter;
  uint32 yPixelsPerMeter;
  uint32 colorsUsed;
  uint32 importantColors;
};

struct BITMAP_HEADER
{
  BITMAP_FILE_HEADER fileHeader;
  BITMAP_DIB_HEADER dibHeader;
};
#pragma pack(pop)

struct Color
{
  Color(uint8 r = 0, uint8 g = 0, uint8 b = 0, uint8 a = 255) 
    : r(r), g(g), b(b), a(a)
  {}

  uint8 r;
  uint8 g;
  uint8 b;
  uint8 a;
};

/*
* Image is set to BGRA
*/
class Image
{
 public:
  Image() = default;

  NODISCARD
  inline uint16 getBytesPerPixel() const
  {
    return m_bpp >> 3;
  }

  NODISCARD
  inline uint32 getPitch() const
  {
    return m_width * getBytesPerPixel();
  }
  
  void create(uint32 width, uint32 height, uint16 bitsPerPixel);
  void decode(const String& filePath);
  void encode(const String& filePath);

  NODISCARD
  Color getPixel(uint32 x, uint32 y) const;

  void setPixel(uint32 x, uint32 y, const Color& color);

  void clearColor(const Color& color);
  

 private:

  uint32 m_width;
  uint32 m_height;
  uint16 m_bpp;

  uint8* m_pixels;
};

