#include <iostream>
#include "MathObjects.h"

int main()
{
  Vector3 v1(2.0f,1.0f,4.0f);
  
  std::cout << v1.x << std::endl;
  std::cout << v1.y << std::endl;
  std::cout << v1.z << std::endl;

  return 0;
}