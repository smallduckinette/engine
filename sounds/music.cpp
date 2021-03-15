#include "engine/sounds/music.h"


#include "engine/sounds/streamable.h"


engine::Music::Music()
{
  alGenBuffers(2, _buffers);
  alGenSources(1, &_source);

  alSource3f(_source, AL_POSITION, 0.0, 0.0, 0.0);
  alSource3f(_source, AL_VELOCITY, 0.0, 0.0, 0.0);
  alSource3f(_source, AL_DIRECTION, 0.0, 0.0, 0.0);
  alSourcef(_source, AL_ROLLOFF_FACTOR, 0.0);
  alSourcei(_source, AL_SOURCE_RELATIVE, AL_TRUE);
}

engine::Music::~Music()
{
  alSourceStop(_source);
  alDeleteBuffers(2, _buffers);
  alDeleteSources(1, &_source);
}

void engine::Music::play(const std::shared_ptr<Streamable> & streamable)
{
  stream(_buffers[0], streamable);
  stream(_buffers[1], streamable);

  alSourceQueueBuffers(_source, 2, _buffers);

  alSourcePlay(_source);
}

void engine::Music::stream(ALuint buffer, const std::shared_ptr<Streamable> & streamable)
{
  _streamBuffer.resize(streamable->getBitrate() * 2 * streamable->getNumChannels());
  size_t size = streamable->stream(&_streamBuffer[0], _streamBuffer.size());

  alBufferData(buffer,
               streamable->getNumChannels() == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
               &_streamBuffer[0],
               size,
               streamable->getBitrate());
}
