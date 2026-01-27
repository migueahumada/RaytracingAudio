#include <iostream>
#include "Audio.h"
#include "MathObjects.h"
#include "DelayLine.h"
#include "Prerequisites.h"
#include "AudioHelpers.h"

#define INPUT_SOUNDFILE "../../rsc/Sound.wav"
#define OUTPUT_SOUNDFILE "../../rsc/TestSoundFile.wav"

static void callback(float* buffer, int channel, int frames , void* cookie)
{
 
}

int main()
{
  

  Audio audioTest;

  audioTest.decode(INPUT_SOUNDFILE);
  std::cout << "--------------------------------" << std::endl;
  std::cout << "       Audio File Decoding" << std::endl;
  std::cout << "--------------------------------" << std::endl;
  std::cout << " > Num channels: " << audioTest.getNumChannels() << std::endl;
  std::cout << " > Bit depth: " << (audioTest.getBytesPerSample() << 3) << std::endl;
  std::cout << " > Bytes per sample: " << audioTest.getBytesPerSample() << std::endl;
  std::cout << " > Sample Rate: " << audioTest.getSampleRate() << std::endl;
  std::cout << " > Num samples: " << audioTest.getTotalNumSamples() << std::endl;
  std::cout << " > Total num frames: " << audioTest.getTotalNumFrames() << std::endl;
  std::cout << " > Total Num samples: " << audioTest.getTotalNumSamples() << std::endl;
  std::cout << " > Volume: " << audioTest.getVolume() << std::endl;
  
  //---------------TESTING 32-BIT FLOAT AUDIO---------------
  Audio audio32Bit;
  audio32Bit.decode("../../rsc/Sound.wav");
  audio32Bit.encode("../../rsc/Test32BitAudioEncoding.wav");

  //---------------TESTING DELAY---------------

  //AudioBuffer with what is inside my wav file
  AudioBuffer audioBuffer(audioTest);

  //Create a DelayLine with the sample size of my .wav
  DelayLine delayLine(audioTest.getTotalNumSamples());

  AudioBuffer delayedAudioBuffer = delayLine.GetProcessedBuffer(audioBuffer,100.0f);
  
  Audio audioDelayed;
  audioDelayed.create(delayedAudioBuffer);
  audioDelayed.encode("../../rsc/TestSoundFileDelayed.wav");

  AudioBuffer summedBuffer = GetSum(audioDelayed,audioBuffer);

  Audio sumAudio;
  sumAudio.create(summedBuffer);
  sumAudio.encode("../../rsc/TestSoundFileSum.wav");

  //---------------TESTING CIRCULAR BUFFER---------------

  Vector<float> data;
  
  RingBuffer rbuffer(4, 2);

  RandomEngine<float> rd;

  for (size_t i = 0; i < rbuffer.getBufferSize(); ++i)
  {
    float* ptr = rbuffer.ptrWrite();
    *ptr = rd.getNumber();
    rbuffer.write();
    data.push_back(*ptr);
  }

  for (size_t i = 0; i < rbuffer.getBufferSize(); ++i)
  {
    //std::cout << *rbuffer.ptrRead() << std::endl;
    rbuffer.read();
  }

  Matrix3x3 m;
  Vector3 v(1.0f,1.0f,1.0f);
  Vector3 vm = m * v;
  Matrix3x3 rotX = GetXRotationMatrix(90);

  Vector3 v1(2.0f, 1.0f, 4.0f);

  return 0;
}