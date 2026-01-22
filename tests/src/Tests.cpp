#include <iostream>
#include "Audio.h"
#include "MathObjects.h"
#include "DelayLine.h"
#include "Prerequisites.h"
#include "AudioHelpers.h"

int main()
{
  Vector3 v1(2.0f, 1.0f, 4.0f);

  std::cout << v1.x << std::endl;
  std::cout << v1.y << std::endl;
  std::cout << v1.z << std::endl;

  Audio audioTest;

  audioTest.decode("../../rsc/IR.wav");
  std::cout << "\nAudio File Decoding" << std::endl;
  std::cout << "################################" << std::endl;
  std::cout << "-Num channels: " << audioTest.getNumChannels() << std::endl;
  std::cout << "-Bit depth: " << (audioTest.getBytesPerSample() << 3) << std::endl;
  std::cout << "-Bytes per sample: " << audioTest.getBytesPerSample() << std::endl;
  std::cout << "-Sample Rate: " << audioTest.getSampleRate() << std::endl;
  std::cout << "-Num samples: " << audioTest.getTotalNumSamples() << std::endl;
  std::cout << "-Total num frames: " << audioTest.getTotalNumFrames() << std::endl;
  std::cout << "-Total Num samples: " << audioTest.getTotalNumSamples() << std::endl;
  std::cout << "-Volume: " << audioTest.getVolume() << std::endl;



  DelayLine delayLine(512);

  AudioBuffer audioBuffer(audioTest);


  delayLine.Process(audioBuffer.m_samples.data(), audioBuffer.m_samples.size());*/

  audioTest.encode("../../rsc/OuputTesting.wav");

  Audio newAudio;
  newAudio.create(audioBuffer);  
  newAudio.encode("../../rsc/OuputTesting.wav");


  

  
  return 0;
}