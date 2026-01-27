#pragma once
#include "Prerequisites.h"
#include "MathObjects.h"
#include <cassert>

namespace FilterType
{
  enum E {
    LOWPASS = 0,
    HIGHPASS,
    BANDPASS
  };
}

#ifdef _WIN64
#define fourccRIFF 'FFIR' //fourcc -> means "four character code"
#define fourccWAVE 'EVAW'
#define fourccFMT  ' tmf'
#define fourccDATA 'atad'
#endif // _WIN32

#ifdef __APPLE__
#define fourccRIFF 'FFIR'
#define fourccWAVE 'EVAW'
#define fourccFMT  ' tmf'
#define fourccDATA 'atad'
#endif // __APPLE__

namespace WAV_FORMAT
{
  enum E
  {
    PCM = 1,
    IEEE_FLOAT = 3
  };
}

struct RIFF_CHUNK
{
  uint32 chunkID;
  uint32 chunkSize;
  uint32 format;
};

struct FMT_SUBCHUNK
{
  uint32 subchunk1ID;
  uint32 subchunk1Size;
  uint16 audioFormat;
  uint16 numChannels;
  uint32 sampleRate;
  uint32 byteRate;
  uint16 blockAlign;
  uint16 bitsPerSample;
};

struct DATA_SUBCHUNK
{
  uint32 subchunk2ID;
  uint32 subchunk2Size;
};

/**
 The WAVE header
 */
struct WAVE_HEADER
{
  RIFF_CHUNK riff;
  FMT_SUBCHUNK fmt;
  DATA_SUBCHUNK data;
};

//Speed of sound at 20°C in m/s, density of 1.2041 pascals
constexpr float SPEED_OF_SOUND {343.21f};

class SoundSource
{
  SoundSource(const Vector3& position) 
  {
    
  };

  Sphere m_sphere;
};



class RingBuffer
{
public:
  explicit RingBuffer(int samples, int frames)
    : m_samples(samples),
      m_bufferSize(samples * frames),
      m_maxFrames(frames),
      m_numFrames(frames),
      m_readIndex(0),
      m_writeIndex(0)
  {
    assert(samples > 0 && "Rinbuffer size should be greater than zero");
    assert((samples & (samples - 1)) == 0 && "Rinbuffer size should be a power of two");
    m_buffer = new float[m_bufferSize];
    memset(*&m_buffer, 0, m_bufferSize * sizeof(float));
  }

  ~RingBuffer()
  {
    if (m_buffer)
    {
      delete[] m_buffer;
      m_buffer = nullptr;
    }
  }

  bool canWrite()
  {
    return m_numFrames != m_maxFrames;
  }
  bool canRead()
  {
    return m_numFrames != 0;
  }

  float* ptrWrite()
  {
    return m_buffer + (m_writeIndex * m_samples);
  }

  float* ptrRead()
  {
    return *&m_buffer + (m_readIndex + m_samples);
  }

  void write()
  {
    m_writeIndex = (m_writeIndex + 1) % m_maxFrames;
    m_numFrames += 1;
  }

  void read()
  {
    m_readIndex = (m_readIndex + 1) % m_maxFrames;
    m_numFrames -=1;
  }

  size_t getBufferSize()
  {
    return m_bufferSize;
  }

private:
  size_t m_bufferSize;
  int m_samples;
  int m_maxFrames;
  int m_numFrames;
  int m_readIndex;
  int m_writeIndex;
  float* m_buffer;
};

