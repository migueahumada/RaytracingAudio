#pragma once
#include "Prerequisites.h"
#include "MathObjects.h"

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
    m_sphere.center = position;
    m_sphere.color = {255.0,0,0};
    m_sphere.coeffs = {0.9,0,0};
    m_sphere.m_radius = 20;
  };

  Sphere m_sphere;
};

struct TimeDelays
{
  Vector<float> m_delays;
};