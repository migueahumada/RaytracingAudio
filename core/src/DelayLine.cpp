#include "DelayLine.h"

DelayLine::DelayLine(int delayInSamples) 
  : m_delayBuffer(delayInSamples, 0),
    m_writeIndex(0)
{
  
}


float DelayLine::Process(float input)
{
  float delayedSample = m_delayBuffer[m_writeIndex];
  m_delayBuffer[m_writeIndex] = input;

  m_writeIndex = (m_writeIndex + 1) % m_delayBuffer.size();

  return delayedSample;
}
