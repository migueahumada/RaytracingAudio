#pragma once
#include "Prerequisites.h"
#include <fstream>
#include <array>

//fourcc -> means "four character code"

#ifdef _WIN32
  #define fourccRIFF 'FFIR'
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


struct WAVE_HEADER
{
  RIFF_CHUNK riff;
  FMT_SUBCHUNK fmt;
  DATA_SUBCHUNK data;
};

template<typename SampleType>
struct AudioBuffer {
  AudioBuffer() = default;
  explicit AudioBuffer(size_t numSamples, size_t numChannels) :
    : numSamples(m_numSamples), numChannels(m_numChannels)
  {
    m_buffer = new SampleType[m_numSamples];
  }

  ~AudioBuffer()
  {
    if (m_buffer)
    {
      delete[] m_buffer;
      m_buffer = nullptr;
    }
  }

  size_t m_numChannels;
  size_t m_numSamples;
  SampleType* m_buffer = nullptr;
};

class Audio
{
 public:
  Audio() = default;

  ~Audio()
  {
    if (m_data)
    {
      delete[] m_data;
      m_data = nullptr;
    }
  }

  void decode(const String& filePath);
  void encode(const String& filePath);

  uint16 BytesPerSample()
  {
    return m_bitsPerSample >> 3;
  }

private:

  void readRiffChunk(std::fstream& file, WAVE_HEADER& waveHeader);
  void readSubchunks(std::fstream& file, WAVE_HEADER& waveHeader);

  uint16 m_numChannels = 0;
  uint32 m_sampleRate = 0;
  uint16 m_bitsPerSample = 0;
  uint32 m_dataBytes = 0;
  
  uint8* m_data = nullptr;
};

/*
* Samples = sampleRate * channels * duration(s);
* 
* NumSamples = (numBytes/bytesPerSample) * channnels;
* 
* NumBytes = (numSamples * bytesPerSample) / channels;
**/

