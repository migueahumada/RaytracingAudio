#include "AudioBuffer.h"

AudioBuffer::AudioBuffer(Audio& audio) 
  : m_channels(audio.getNumChannels()),
    m_sampleRate(audio.getSampleRate()),
    m_bitDepth(audio.getBytesPerSample() * 8)
{
  for (uint32 frame = 0; frame < audio.getTotalNumFrames(); ++frame)
  {
    for (uint32 channel = 0; channel < audio.getNumChannels(); ++channel)
    {
      float inSample = audio.getFrameSample(channel, frame);
      m_samples.push_back(inSample);
    }
  }
}

AudioBuffer AudioBuffer::operator+(const AudioBuffer& other)
{
  AudioBuffer otherBuffer = other;
  AudioBuffer resultBuffer;

  if (m_samples.size() > otherBuffer.m_samples.size())
  {
    otherBuffer.m_samples.resize(m_samples.size());
    resultBuffer.m_samples.resize(m_samples.size());
    resultBuffer.m_channels = m_channels;
    resultBuffer.m_sampleRate = m_sampleRate;
    resultBuffer.m_bitDepth = m_bitDepth;
  }
  else
  {
    m_samples.resize(other.m_samples.size());
    resultBuffer.m_samples.resize(other.m_samples.size());
    resultBuffer.m_channels = other.m_channels;
    resultBuffer.m_sampleRate = other.m_sampleRate;
    resultBuffer.m_bitDepth = other.m_bitDepth;
  }

  for (size_t i = 0; i < resultBuffer.m_samples.size(); ++i)
  {
    float resultSample = other.m_samples[i] + m_samples[i];
    resultBuffer.m_samples[i] = clamp(resultSample, -1.0f, 1.0f);
  }

  return resultBuffer;
  
}


void AudioBuffer::scale(float scalar)
{
  for (size_t i = 0; i < m_samples.size(); i++)
  {
    m_samples[i] *= scalar;
  }
}

AudioBuffer AudioBuffer::sum(const AudioBuffer& other)
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

  for (size_t i = 0; i < tmpBuffer.m_samples.size(); ++i)
  {
    float resultSample = tmpBuffer.m_samples[i] + thisBuffer.m_samples[i];
    resultBuffer.m_samples[i] = clamp(resultSample, -1.0f, 1.0f);
  }

  return resultBuffer;
}



/**
* There's a relationship between the time, the sampleRate and the samples
* 1 sec = 48000 samples
* 1ms = 0.001 s = 48 samples
* 0.0001s = 4.8 samples
* 
* 0.001s * 48000samp / 1s = 48 samp
*/
AudioBuffer AudioBuffer::getAudioBuferWithTimeOffset(float timeInMilliseconds)
{
  //Calculate the samples offset
  float timeInSeconds = timeInMilliseconds * 0.001f;
  int32 samplesToOffset = static_cast<int32>(timeInSeconds * m_sampleRate * m_channels);

  AudioBuffer tmpBuffer = *this;
  tmpBuffer.m_samples.clear();
  
  tmpBuffer.m_samples.resize(m_samples.size() + samplesToOffset);

  for (size_t i = 0; i < m_samples.size(); ++i)
  {
    tmpBuffer.m_samples[i+samplesToOffset] = m_samples[i];
  }

  return tmpBuffer;


}

void AudioBuffer::setTimeOffset(float timeInMilliseconds)
{
  float timeInSeconds = timeInMilliseconds * 0.001f;
  int32 samplesToOffset = static_cast<int32>(timeInSeconds * m_sampleRate * m_channels);

  Vector<float> tmpBuffer;

  tmpBuffer.resize(m_samples.size() + samplesToOffset);

  for (size_t i = 0; i < m_samples.size(); ++i)
  {
    tmpBuffer[i + samplesToOffset] = m_samples[i];
  }

  m_samples = tmpBuffer;
  
}

void AudioBuffer::setZero()
{
  for (auto& samples : m_samples)
  {
    samples = 0.0f;
  }
}

