#pragma once
#include "Prerequisites.h"

class DelayLine
{
 public:
  explicit DelayLine(int delayInSamples);

  float Process(float input);

 private:
  Vector<float> m_delayBuffer;
  size_t m_writeIndex;

};

