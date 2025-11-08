#include "Audio.h"
#include <assert.h>
#include <limits>
#include <iostream>

void Audio::create(uint32 sampleRate, 
                   uint16 bitDepth, 
                   uint16 numChannels, 
                   uint32 audioDataSize)
{

  m_sampleRate = sampleRate;
  m_bitsPerSample = bitDepth;
  m_numChannels = numChannels;
  m_dataSize = audioDataSize;

}

void Audio::decode(const String& filePath)
{
  std::fstream file(filePath, std::ios::binary | std::ios::in);

  if (!file.is_open())
  {
    return;
  }

  WAVE_HEADER waveHeader{0};

  readRiffChunk(file, waveHeader);

  readSubchunks(file, waveHeader);

  create(waveHeader.fmt.sampleRate,
         waveHeader.fmt.bitsPerSample,
         waveHeader.fmt.numChannels,
         waveHeader.data.subchunk2Size);

  file.close();

  printf("Succesfully decoded an audio file.\n");
}

void Audio::encode(const String& filePath)
{

  std::fstream file(filePath, std::ios::binary | std::ios::out);

  if (!file.is_open())
  {
    return;
  }

  WAVE_HEADER waveHeader{0};
  waveHeader.riff.chunkID = fourccRIFF;
  waveHeader.riff.chunkSize = sizeof(WAVE_HEADER) - 8 + m_dataSize;
  waveHeader.riff.format = fourccWAVE;

  waveHeader.fmt.subchunk1ID = fourccFMT;
  waveHeader.fmt.subchunk1Size = sizeof(FMT_SUBCHUNK) - 8;
  waveHeader.fmt.audioFormat = 1;
  waveHeader.fmt.numChannels = m_numChannels;
  waveHeader.fmt.sampleRate = m_sampleRate;
  waveHeader.fmt.byteRate = m_sampleRate * m_numChannels * getBytesPerSample();
  waveHeader.fmt.blockAlign = m_numChannels * getBytesPerSample();
  waveHeader.fmt.bitsPerSample = m_bitsPerSample;

  waveHeader.data.subchunk2ID = fourccDATA;
  waveHeader.data.subchunk2Size = m_dataSize;

  file.write(reinterpret_cast<char*>(&waveHeader), sizeof(waveHeader));

  file.write(reinterpret_cast<char*>(&*m_data), m_dataSize);

  file.close();

  printf("Succesfully encoded an audio file.\n");
}

/*
*              c0   c1
*    frame 0  | x | x |
*    frame 1  | x | x |
*    frame 2  | x | x |
* 
*/

// This is like using a get Pixel function
float Audio::getFrameSample(int channelIndex,
                            int frameIndex)
{
  assert(channelIndex < m_numChannels &&
         "Channel should not be greater than the number of channels");
  assert(channelIndex >= 0 && "Channel should not be less than 0");

  int samplePos = (frameIndex * m_numChannels + channelIndex) *  getBytesPerSample();
  
  float sampleValue = 0;

  if (m_bitsPerSample == 8)
  {
    int8 rawSample = *reinterpret_cast<int8*>(&m_data[samplePos]);

    sampleValue = static_cast<float>(rawSample) / std::numeric_limits<int8>::max();

  }

  if (m_bitsPerSample == 16)
  {
    int16 rawSample = *reinterpret_cast<int16*>(&m_data[samplePos]);

    sampleValue = static_cast<float>(rawSample)/ std::numeric_limits<int16>::max();

  }

  if (m_bitsPerSample == 32)
  {
    int32 rawSample = *reinterpret_cast<int32*>(&m_data[samplePos]);
    sampleValue = static_cast<float>(rawSample) / std::numeric_limits<int32>::max();

  }

  return sampleValue;

}

void Audio::setFrameSample(int channelIndex,
                           int frameIndex,
                           float sampleValue)
{
  
  assert(channelIndex < m_numChannels &&
    "Channel should not be greater than the number of channels");
  assert(channelIndex >= 0 && "Channel should not be less than 0");

  int samplePos = (frameIndex * m_numChannels + channelIndex) * getBytesPerSample();

  if (m_bitsPerSample == 8)
  {
    int8 out = static_cast<int8>(std::round(sampleValue * static_cast<float>(std::numeric_limits<int8>::max())));
    memcpy(&m_data[samplePos], &out, sizeof(int8));
  }

  if (m_bitsPerSample == 16)
  {

    int16 out = static_cast<int16>(std::round(sampleValue * static_cast<float>(std::numeric_limits<int16>::max())));
    memcpy(&m_data[samplePos], &out, sizeof(int16));
    
  }

  if (m_bitsPerSample == 32)
  {
    int32 out = static_cast<int32>(std::round(sampleValue * static_cast<float>(std::numeric_limits<int32>::max())));
    memcpy(&m_data[samplePos], &out, sizeof(int32));

  }

}


void Audio::processAudio()
{
  
  printf("The number of byte data is: %d\n", m_dataSize);
  printf("The number of samples is: %d\n", getTotalNumSamples());
  printf("The number of frames is: %d\n",getTotalNumFrames());


  for (int frame = 0; frame < getTotalNumFrames(); ++frame)
  {
    for (int channel = 0; channel < getNumChannels(); ++channel)
    {
      float inSample = getFrameSample(channel, frame);
      setFrameSample(channel, frame, inSample * 0.5f);
    }
  }
}



void Audio::readRiffChunk(std::fstream& file,
                          WAVE_HEADER& waveHeader)
{
  uint32 tempBuffer;

  //IS RIFF?
  file.read(reinterpret_cast<char*>(&tempBuffer), sizeof(tempBuffer));
  if (tempBuffer != fourccRIFF)
  {
    return;
  }

  waveHeader.riff.chunkID = tempBuffer;

  //SIZE OF RIFF CHUNK
  file.read(reinterpret_cast<char*>(&tempBuffer), sizeof(tempBuffer));

  waveHeader.riff.chunkSize = tempBuffer;

  //IS FMT?
  file.read(reinterpret_cast<char*>(&tempBuffer), sizeof(tempBuffer));
  if (tempBuffer != fourccWAVE)
  {
    return;
  }

  waveHeader.riff.format = tempBuffer;
}

void Audio::readSubchunks(std::fstream& file,
                          WAVE_HEADER& waveHeader)
{

  //ITERATE THROUGH SUBCHUNKS
  while (file.peek() != EOF)
  {

    uint32 tempBuffer;
    file.read(reinterpret_cast<char*>(&tempBuffer), sizeof(tempBuffer));

    switch (tempBuffer)
    {
    case fourccFMT:

      file.seekg(-static_cast<std::streamoff>(sizeof(tempBuffer)), std::ios::cur);

      file.read(reinterpret_cast<char*>(&waveHeader.fmt), sizeof(waveHeader.fmt));

      break;

    case fourccDATA:

      file.seekg(-static_cast<std::streamoff>(sizeof(tempBuffer)), std::ios::cur);

      file.read(reinterpret_cast<char*>(&waveHeader.data), sizeof(waveHeader.data));

      m_data = new uint8[waveHeader.data.subchunk2Size];

      file.read(reinterpret_cast<char*>(&*m_data), waveHeader.data.subchunk2Size);
      

      break;

    default:
      file.read(reinterpret_cast<char*>(&tempBuffer), sizeof(tempBuffer));

      file.seekg(static_cast<std::streamoff>(tempBuffer), std::ios::cur);
      break;
    }
  }
}

