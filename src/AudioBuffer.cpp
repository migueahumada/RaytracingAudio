#include "AudioBuffer.h"

AudioBuffer::AudioBuffer(Audio& audio) 
  : m_channels(audio.getNumChannels()),
    m_sampleRate(audio.getSampleRate()),
    m_bitDepth(audio.getBytesPerSample() * 8)
{
  for (size_t frame = 0; frame < audio.getTotalNumFrames(); ++frame)
  {
    for (size_t channel = 0; channel < audio.getNumChannels(); ++channel)
    {
      float inSample = audio.getFrameSample(channel, frame);
      m_samples.push_back(inSample);
    }
  }
}

AudioBuffer AudioBuffer::operator+(const AudioBuffer& other)
{
  AudioBuffer tmpBuffer = other;
  AudioBuffer thisBuffer = *this;
  AudioBuffer resultBuffer;

  if (this->m_samples.size() > other.m_samples.size())
  {
    tmpBuffer.m_samples.resize(thisBuffer.m_samples.size());
    resultBuffer.m_samples.resize(thisBuffer.m_samples.size());
    resultBuffer.m_channels = thisBuffer.m_channels;
    resultBuffer.m_sampleRate = thisBuffer.m_sampleRate;
    resultBuffer.m_bitDepth = thisBuffer.m_bitDepth;
  }
  else
  {
    thisBuffer.m_samples.resize(tmpBuffer.m_samples.size());
    resultBuffer.m_samples.resize(tmpBuffer.m_samples.size());
    resultBuffer.m_channels = tmpBuffer.m_channels;
    resultBuffer.m_sampleRate = tmpBuffer.m_sampleRate;
    resultBuffer.m_bitDepth = tmpBuffer.m_bitDepth;
  }

  for (size_t i = 0; i < tmpBuffer.m_samples.size(); i++)
  {
    resultBuffer.m_samples[i] = tmpBuffer.m_samples[i] + thisBuffer.m_samples[i];
  }

  return resultBuffer;
}