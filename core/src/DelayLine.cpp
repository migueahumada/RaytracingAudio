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

void DelayLine::Process(float* inBuffer, int bufferSize, float delayTime, float sampleRate)
{
  float* inReadPtr = inBuffer;
  float* inWritePtr = inBuffer;

  m_currentDelayTime = delayTime;
  m_floatSampleRate = sampleRate;

  int samplesDelayed = static_cast<int>(m_currentDelayTime * m_floatSampleRate);
  
  while (--bufferSize >= 0)
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

void DelayLine::Process(AudioBuffer& audioBuffer, float delayTimeInMS)
{
  m_currentDelayTime = delayTimeInMS * 0.001f;
  m_floatSampleRate = static_cast<float>(audioBuffer.m_sampleRate);

  float* inReadPtr = &audioBuffer.m_samples[0];
  float* inWritePtr = &audioBuffer.m_samples[0];

  int numSampleFrames = static_cast<int>(audioBuffer.m_samples.size());

  int samplesDelayed = static_cast<int>(m_currentDelayTime * m_floatSampleRate * audioBuffer.m_channels);

  while ((--numSampleFrames) >= 0)
  {
    m_buffer[m_writePos] = *inReadPtr;
    ++inReadPtr;
    ++m_writePos;
    m_writePos = m_writePos % m_buffer.size(); 

    int readPos = (m_writePos + (audioBuffer.m_channels - 1) - samplesDelayed) % m_buffer.size();
    *inWritePtr = m_buffer[readPos];
    ++inWritePtr;

  }
}


