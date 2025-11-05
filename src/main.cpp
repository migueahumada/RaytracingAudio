#include "Audio.h"
#include <vector>

int main()
{
  Audio audio;

  audio.decode("/Users/miko/Documents/Coding/RaytracingAudio/rsc/Loquendo.wav");

  audio.processAudio();

  audio.encode("/Users/miko/Documents/Coding/RaytracingAudio/rsc/out2.wav");

  

  return 0;
}
