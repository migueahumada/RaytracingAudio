#include <iostream>
#include "Audio.h"
#include "MathObjects.h"

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

  return 0;
}