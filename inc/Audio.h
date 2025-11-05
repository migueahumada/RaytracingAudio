#pragma once
#include "Prerequisites.h"
#include <fstream>

/*
* Size of the frame.
* Each frame has a channel and a sample.
*
* Example: If I have a Stereo 44.1 16bits soundfile
*         the audio data will be organized.
*
*         Frame1  Frame2
*         v       v
*         |-------|-------|
*          --- --- --- --- --- ---
* sample->| 0 | 1 | 2 | 3 | 4 | 5 |
*          --- --- --- --- --- ---
*           ^
*           L channel
 
  - Data -> array of bytes.
  - Sample -> the normalization of the data.
  - Frame -> a container of samples which size is the number of channels.
  
 
*/

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

/**
 The header
 */
struct WAVE_HEADER
{
  RIFF_CHUNK riff;
  FMT_SUBCHUNK fmt;
  DATA_SUBCHUNK data;
};

#pragma pack(push, 4)
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

  void create(uint32 sampleRate,
              uint16 bitDepth,
              uint16 numChannels,
              uint32 audioDataSize);
  void decode(const String& filePath);
  void encode(const String& filePath);

  /**
  * Number of bytes per samples
  */
  NODISCARD
  inline const uint16 getBytesPerSample() const
  {
    return m_bitsPerSample >> 3;
  }

  /**
  * Total number of frames
  */
  NODISCARD 
  inline const uint16 getNumChannels() const
  {
    return m_numChannels;
  }

  /**
  * Number of all samples in the audioData array.
  */
  NODISCARD
  inline const uint32 getNumSamples() const
  {
    return (m_dataSize / getBytesPerSample())* m_numChannels;
  }
  
  /**
  * Frame Size = channels * bytesPerSample
  */
  NODISCARD
  inline const uint16 getFrameSize() const
  {
    return getBytesPerSample() * m_numChannels;
  }

  /**
  * Total number of frames
   
  */
  NODISCARD
  inline const uint32 getTotalNumFrames() const
  {
     return getNumSamples() / m_numChannels;
  }

  /**
    Gets the frame Sample value in floats
  */
  NODISCARD
  float getFrameSample(int channelIndex, int frameIndex);

  /**
    Sets the frame sample value as a float
  */
  void setFrameSample(int channelIndex,
                      int frameIndex,
                      float sampleValue); 
  
  void processAudio();

private:

  void readRiffChunk(std::fstream& file, 
                     WAVE_HEADER& waveHeader);

  void readSubchunks(std::fstream& file, 
                     WAVE_HEADER& waveHeader);

  void createAudioSamples(uint16 precision,
                          uint32 sampleRate,
                          uint16 samplesPerFrame);

  
  uint32 m_sampleRate = 0;
  uint32 m_dataSize = 0;
  uint16 m_bitsPerSample = 0;
  uint16 m_numChannels = 0;

  uint8* m_data = nullptr;
  
  
};
#pragma pack(pop)

/*
* Samples = sampleRate * channels * duration(s);
* 
* NumSamples = (numBytes/bytesPerSample) * channnels;
* 
* NumBytes = (numSamples * bytesPerSample) / channels;
**/

