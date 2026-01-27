#pragma once
#include "Prerequisites.h"
#include "Audio.h"

class Audio;

class AudioBuffer
{
 public:
  AudioBuffer() = default;
  AudioBuffer(uint16 channels, 
              uint32 sampleRate, 
              uint16 bitDepth) 
    : m_channels(channels), 
      m_sampleRate(sampleRate), 
      m_bitDepth(bitDepth)
  {}
  
  AudioBuffer(Audio& audio);

  AudioBuffer operator+(const AudioBuffer& other);
  
  void scale(float scalar);
  void setZero();

  AudioBuffer sum(const AudioBuffer& other);
  
  AudioBuffer getAudioBuferWithTimeOffset(float timeInMilliseconds);
  
  void setTimeOffset(float timeInMilliseconds);

  NODISCARD 
  inline uint32 getSampleRate()
  {
    return m_sampleRate;
  }

  NODISCARD
  inline uint16 getNumChannels()
  {
    return m_channels;
  }

  NODISCARD
  inline uint16 getBitDepth()
  {
    return m_bitDepth;
  }

 public:
  Vector<float> m_samples;
  uint32 m_sampleRate{ 0 };
  uint16 m_channels {0};
  uint16 m_bitDepth = {0};
};

static AudioBuffer GetSum(const AudioBuffer& x, const AudioBuffer& y)
{
  {
    AudioBuffer xBuffer = x;
    AudioBuffer yBuffer = y;
    AudioBuffer resultBuffer;

    if (yBuffer.m_samples.size() > xBuffer.m_samples.size())
    {
      xBuffer.m_samples.resize(yBuffer.m_samples.size());
      resultBuffer.m_samples.resize(xBuffer.m_samples.size());
      resultBuffer.m_channels = xBuffer.m_channels;
      resultBuffer.m_sampleRate = xBuffer.m_sampleRate;
      resultBuffer.m_bitDepth = xBuffer.m_bitDepth;
    }
    else
    {
      yBuffer.m_samples.resize(xBuffer.m_samples.size());
      resultBuffer.m_samples.resize(yBuffer.m_samples.size());
      resultBuffer.m_channels = yBuffer.m_channels;
      resultBuffer.m_sampleRate = yBuffer.m_sampleRate;
      resultBuffer.m_bitDepth = yBuffer.m_bitDepth;
    }

    for (size_t i = 0; i < resultBuffer.m_samples.size(); ++i)
    {
      float resultSample = xBuffer.m_samples[i] + yBuffer.m_samples[i];
      resultBuffer.m_samples[i] = clamp(resultSample, -1.0f, 1.0f);
    }

    return resultBuffer;
  }
}

