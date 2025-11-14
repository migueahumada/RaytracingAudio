#include "Audio.h"

#ifdef _WIN32
  #define INPATH "../rsc/Woosh.wav"
  #define OUTPATH  "../rsc/out2.wav"
#endif

#ifdef __APPLE__
  #define INPATH "../../rsc/SoundFile_01.wav"
  #define OUTPATH "../../rsc/out2.wav"
#endif

#ifdef __UNIX__
  #define INPATH "../../rsc/SoundFile_01.wav"
  #define OUTPATH "../../rsc/out2.wav"
#endif


int main()
{
  Audio audio;

  audio.decode(INPATH);

  audio.processAudio();

  audio.biquad(FilterType::BANDPASS,500.0,0.707f);

  audio.encode(OUTPATH);

  

  return 0;
}
