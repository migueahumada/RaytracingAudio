#include "Viewport.h"

Viewport::Viewport(uint16 width, 
                   uint16 height) 
  : m_width(width),
    m_height(height),
    m_ratio((REAL_TYPE)height / 100),
    m_upperLeftCorner(Vector3(50, 50, 100))
{
}
