#include "Audio.h"
#include <vector>

int main()
{
  Audio audio;

  audio.decode("../rsc/Loquendo.wav");

  audio.encode("../rsc/out2.wav");

  

  return 0;
}