#include <iostream>
#include "Audio.h"
#include "MathObjects.h"
#include "DelayLine.h"
#include "Prerequisites.h"

#define MAX_BUFFER_LENGTH 16
#define BIT_MASK (MAX_BUFFER_LENGTH - 1) 



class FlexibleDelayLine
{
 public:
  explicit FlexibleDelayLine(size_t size/*, int delayTime, int sampleRate*/)
    : m_writePos(0)
      //m_currentDelayTime(delayTime),
      //m_floatSampleRate(sampleRate) 
  {
    m_buffer.resize(size);
  }
  
  void Process(float* inBuffer, int numSamplesFrames)
  {
    

    float* inReadPtr = inBuffer;
    float* inWritePtr = inBuffer;

    int samplesDelayed = static_cast<int>(m_currentDelayTime * m_floatSampleRate);

    while ( --numSamplesFrames >= 0)
    {
      m_buffer[m_writePos] = *inReadPtr;
      ++inReadPtr;
      ++m_writePos;
      m_writePos &= (m_buffer.size()- 1);

      int readPos = (m_writePos - samplesDelayed) & (m_buffer.size() -1);
      *inWritePtr = m_buffer[readPos];
      ++inWritePtr;
      
    }
  }

 //private:
  int   m_writePos;
  Vector<float> m_buffer;
  float m_currentDelayTime = 5.0f;
  float m_floatSampleRate = 1.0f;
  
};

int main()
{
  Vector3 v1(2.0f, 1.0f, 4.0f);

  std::cout << v1.x << std::endl;
  std::cout << v1.y << std::endl;
  std::cout << v1.z << std::endl;

  Audio audioTest;

  audioTest.decode("../../rsc/Woosh.wav");
  std::cout << "\nAudio File Decoding" << std::endl;
  std::cout << "################################" << std::endl;
  std::cout << "-Num channels: " << audioTest.getNumChannels() << std::endl;
  std::cout << "-Bit depth: " << (audioTest.getBytesPerSample() << 3) << std::endl;
  std::cout << "-Bytes per sample: " << audioTest.getBytesPerSample() << std::endl;
  std::cout << "-Num samples: " << audioTest.getTotalNumSamples() << std::endl;
  std::cout << "-Total num frames: " << audioTest.getTotalNumFrames() << std::endl;
  std::cout << "-Total Num samples: " << audioTest.getTotalNumSamples() << std::endl;
  std::cout << "-Volume: " << audioTest.getVolume() << std::endl;



  FlexibleDelayLine delayLine(512);

  RandomEngine<float> rd;

  AudioBuffer audioBuffer(audioTest);


  delayLine.Process(audioBuffer.m_samples.data(), audioBuffer.m_samples.size());

  Audio newAudio;
  newAudio.create(audioBuffer);
  
  newAudio.encode("newAUdio.wav");


  

  
  return 0;
}