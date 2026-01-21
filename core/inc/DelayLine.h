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

template<typename T>
class CircularBuffer
{
 public:
  explicit CircularBuffer(size_t size) 
    : m_readIndex(0),
      m_writeIndex(0)
  {
    assert(size > 0 && "size should be greater than zero");
    assert((size & (size-1)) == 0 && "size should be a power of two");
    m_buffer.resize(size);
  }

  void write(const T& value)
  {
    m_buffer[m_writeIndex] = value;
    m_writeIndex = (m_writeIndex + 1) % m_buffer.size();
  }

  T read()
  {
    m_buffer[m_read]
  }

 private:
  Vector<T> m_buffer;
  int m_readIndex;
  int m_writeIndex;
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