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

 public:
  Vector<float> m_samples;
  uint16 m_channels {0};
  uint32 m_sampleRate {0};
  uint16 m_bitDepth = {0};
};

