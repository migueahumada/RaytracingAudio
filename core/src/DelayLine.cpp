#include "DelayLine.h"
#include "AudioBuffer.h"

DelayLine::DelayLine(size_t size/*, int delayTime, int sampleRate*/)
  : m_writePos(0)
{
  m_buffer.resize(size);
}


void DelayLine::Process(float* inBuffer, int numSamplesFrames)
{

  float* inReadPtr = inBuffer;
  float* inWritePtr = inBuffer;

  int samplesDelayed = static_cast<int>(m_currentDelayTime * m_floatSampleRate);

  while (--numSamplesFrames >= 0)
  {
    m_buffer[m_writePos] = *inReadPtr;
    ++inReadPtr;
    ++m_writePos;
    m_writePos &= (m_buffer.size() - 1);

    int readPos = (m_writePos - samplesDelayed) & (m_buffer.size() - 1);
    *inWritePtr = m_buffer[readPos];
    ++inWritePtr;

  }
}

void DelayLine::Process(AudioBuffer& audioBuffer, float delayTime)
{
  m_currentDelayTime = delayTime;
  m_floatSampleRate = audioBuffer.m_sampleRate;
  int numSampleFrames = audioBuffer.m_samples.size();

  float* inReadPtr = audioBuffer.m_samples.data();
  float* inWritePtr = audioBuffer.m_samples.data();

  int samplesDelayed = static_cast<int>(m_currentDelayTime * m_floatSampleRate);

  while (--numSampleFrames >= 0)
  {
    m_buffer[m_writePos] = *inReadPtr;
    ++inReadPtr;
    ++m_writePos;
    m_writePos &= (m_buffer.size() - 1);

    int readPos = (m_writePos - samplesDelayed) & (m_buffer.size() - 1);
    *inWritePtr = m_buffer[readPos];
    ++inWritePtr;

  }
}


