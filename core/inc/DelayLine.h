#pragma once
#include "Prerequisites.h"

class AudioBuffer;

class DelayLine
{
public:
  
  DelayLine(size_t size);
  

  void Process(float* inBuffer, int numSamplesFrames);
  void Process(AudioBuffer& audioBuffer, float delayTime);
  void Process(float* inBuffer, int bufferSize, float delayTime, float sampleRate);

  NODISCARD 
  inline Vector<float> getSamplesBuffer()
  {
    return m_buffer;
  }

  
  int m_writePos;
  float m_currentDelayTime = 1.0f;
  Vector<float> m_buffer;
  float m_floatSampleRate = 2.0f;
};




//struct CircularBuffer
//{
//  CircularBuffer(int samples,
//                 int frames)
//    : buffer(nullptr),
//    samples(samples),
//    maxFrames(frames),
//    read(0),
//    write(0),
//    numFrames(frames)
//  {
//    buffer = new float[maxFrames * frames];
//    memset(buffer, 0, maxFrames * frames * sizeof(float));
//  }
//
//  bool canWrite()
//  {
//    return numFrames != maxFrames;
//  }
//
//  float* writeBuffer()
//  {
//    return buffer + (write * samples);
//  }
//
//  void finish_write()
//  {
//    write = (write + 1) % maxFrames;
//    numFrames += 1;
//  }
//
//  bool canRead()
//  {
//    return numFrames != 0;
//  }
//
//  float* readBuffer()
//  {
//    return buffer + (read * samples);
//  }
//
//  void finishRead()
//  {
//    read = (read + 1) % maxFrames;
//    numFrames -= 1;
//  }
//
//  ~CircularBuffer()
//  {
//    if (buffer)
//    {
//      delete buffer;
//      buffer = nullptr;
//    }
//
//  }
//
//private:
//  float* buffer;
//  int samples;
//  int maxFrames;
//  int read;
//  int write;
//  int numFrames;
//};