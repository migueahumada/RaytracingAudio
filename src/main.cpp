#include "Audio.h"
#include <vector>

int main()
{
  Audio audio;

  audio.decode("../rsc/SoundFile_01.wav");

  audio.processAudio();

  audio.encode("../rsc/out2.wav");

  

  return 0;
}