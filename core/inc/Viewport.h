#pragma once
#include "Prerequisites.h"
#include "MathObjects.h"

/*
  Creates a viewport that can easily be converted into an image.
*/

class Viewport
{
 public:
  Viewport(uint16 width, uint16 height);

  REAL_TYPE m_ratio = 0;
  uint16 m_width;
  uint16 m_height;
  Vector3 m_upperLeftCorner;
};

