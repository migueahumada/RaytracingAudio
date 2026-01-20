#include <iostream>
#include "Audio.h"
int main()
{
  
  Audio audioTest;

  audioTest.decode("../../rsc/Woosh.wav");
  std::cout << "\nAudio File Decoding" << std::endl;
  std::cout << "################################" << std::endl;
  std::cout << "-Num channels: " << audioTest.getNumChannels() << std::endl;
  std::cout << "-Bit depth: " << (audioTest.getBytesPerSample() << 3) << std::endl;
  std::cout << "-Bytes per sample: " << audioTest.getBytesPerSample() << std::endl;
  std::cout << "-Num samples: "        << audioTest.getTotalNumSamples() << std::endl;
  std::cout << "-Total num frames: "   << audioTest.getTotalNumFrames() << std::endl;
  std::cout << "-Total Num samples: "  << audioTest.getTotalNumSamples() << std::endl;
  std::cout << "-Volume: "             << audioTest.getVolume() << std::endl;
  

  return 0;
}