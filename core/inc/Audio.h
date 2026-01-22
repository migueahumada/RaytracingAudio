#pragma once
#include "Prerequisites.h"
#include "MathObjects.h"
#include "AudioHelpers.h"
#include "AudioBuffer.h"
#include <fstream>
#include <cassert>

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
*
* - Data -> array of bytes.
* - Sample -> the normalization of the data.
* - Frame -> a container of samples which size is the number of channels.
* 
*
* Formulas:
*  Duration = channels * sampleRate * bitdepth
*
*  Samples = sampleRate * channels * duration(s);
*
*  NumBytes = numSamples * bytesPerSample;
*
*  DataSize = sampleRate * channels * duration
*
*/

class AudioBuffer;

#pragma pack(push, 8)
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

  
  /*
  * Creates an audio object with some duration.
  */
  void create(uint32 sampleRate,
              uint16 bitDepth,
              uint16 numChannels,
              uint32& durationInMS);
  
  /*
  * Creates an audio object from an AudioBuffer
  */
  void create(AudioBuffer& audioBuffer);
  
  /**
  * Reads the contents of a wave file 
  **/
  void decode(const String& filePath);
  
  /**
  * Outputs a wave file with the data from this class
  **/
  void encode(const String& filePath);

  /**
  * Number of bytes per samples
  */
  NODISCARD
  inline const uint16 getBytesPerSample() const
  {
    //assert(m_dataSize > 0 && "Couldn't get the BytesPerSample");
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
  inline const size_t getTotalNumSamples() const
  {
    //assert(m_dataSize > 0 && "Size of audio data is 0");
    return m_dataSize / getBytesPerSample();
  }

  /**
  * Total number of frames
  */
  NODISCARD
  inline const size_t getTotalNumFrames() const
  {
     assert(m_dataSize > 0 && "Couldn't get the TotalNumFrames.");
     return getTotalNumSamples() / m_numChannels;
  }

  NODISCARD
  inline const uint32 getSampleRate() const
  {
    return m_sampleRate;
  }

  /**
    Gets the frame Sample value in floats
  */
  NODISCARD
  float getFrameSample(uint32 channelIndex, uint32 frameIndex);

  /**
    Sets the frame sample value as a float
  */
  void setFrameSample(uint32 channelIndex,
                      uint32 frameIndex,
                      float sampleValue); 
  
  void processAudio();

  inline NODISCARD float getVolume() const
  {
    return m_volume;
  }

  void setVolume(float newVolume);
  

  void sine(float amp = 0.5f, 
            float freq = 440.0f,
            float phase = 0.0f);

  void phoneDial(float amp = 0.5f,
                 float freq = 440.0f,
                 float phase = 0.0f);

  void lowpass(float cutoff, float Q);
  void highpass(float cutoff, float Q);
  void bandpass(float cutoff, float Q);

  void biquad(FilterType::E type,float cutoff, float Q);
  void butterworth(FilterType::E type, float cutoff);


  void digitalIntegrator();

private:

  /*
  * Used to create the object
  */
  void create(uint32 sampleRate,
    uint16 bitDepth,
    uint16 numChannels,
    size_t audioDataSize);

  /*
  * Reads the RIFF chunk from an wavefile
  */
  void readRiffChunk(std::fstream& file, 
                     WAVE_HEADER& waveHeader);
  /*
  * Reads the subchunks, only FMT and DATA.
  * TODO: Add more subchunks for metadata
  */
  void readSubchunks(std::fstream& file, 
                     WAVE_HEADER& waveHeader);

  /**
  * 1 -> PCM encoding
  * 3 -> float encoding
  */
  NODISCARD
  inline WAV_FORMAT::E getAudioFormat()
  {
    return (m_bitsPerSample == 32) ? 
      WAV_FORMAT::IEEE_FLOAT : WAV_FORMAT::PCM;
  }

  size_t m_dataSize {0};
  uint32 m_sampleRate {0};
  uint16 m_bitsPerSample {0};
  uint16 m_numChannels {0};
  uint8* m_data {nullptr};
  float m_volume {1.0f};
  
  
};
#pragma pack(pop)


