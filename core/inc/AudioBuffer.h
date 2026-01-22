#pragma once
#include "Prerequisites.h"
#include "Audio.h"

class Audio;

class AudioBuffer
{
 public:
  AudioBuffer() = default;
  AudioBuffer(uint16 channels, uint32 sampleRate, uint16 bitDepth) :
              m_channels(channels), m_sampleRate(sampleRate), m_bitDepth(bitDepth)
  {}
  
  AudioBuffer(Audio& audio);

  AudioBuffer operator+(const AudioBuffer& other);
  
  void scale(float scalar);

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

