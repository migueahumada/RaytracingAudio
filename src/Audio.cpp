#include "Audio.h"

void Audio::create(uint32 sampleRate, uint16 bitDepth, uint16 numChannels, uint32 audioDataSize)
{

  m_sampleRate = sampleRate;
  m_bitsPerSample = bitDepth;
  m_numChannels = numChannels;
  m_dataSize = audioDataSize;

  switch (m_bitsPerSample)
  {
  case 8:
    break;
  case 16:
    break;
  case 32:
    break;
  default:
    break;
  }
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
  waveHeader.fmt.byteRate = m_sampleRate * m_numChannels * BytesPerSample();
  waveHeader.fmt.blockAlign = m_numChannels * BytesPerSample();
  waveHeader.fmt.bitsPerSample = m_bitsPerSample;

  waveHeader.data.subchunk2ID = fourccDATA;
  waveHeader.data.subchunk2Size = m_dataSize;

  file.write(reinterpret_cast<char*>(&waveHeader), sizeof(waveHeader));

  file.write(reinterpret_cast<char*>(&*m_data), m_dataSize);

  file.close();

  printf("Succesfully encoded an audio file.\n");
}

void Audio::readRiffChunk(std::fstream& file, WAVE_HEADER& waveHeader)
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

void Audio::readSubchunks(std::fstream& file, WAVE_HEADER& waveHeader)
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
