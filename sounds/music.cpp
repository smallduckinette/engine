#include "engine/sounds/music.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>

#include "engine/sounds/streamable.h"


engine::Music::Music(boost::asio::io_context * ioContext):
  _ioContext(ioContext),
  _strand(boost::asio::make_strand(*_ioContext)),
  _bufferDuration(std::chrono::seconds(5)),
  _bufferRefreshInterval(std::chrono::milliseconds(_bufferDuration) / 2),
  _timer(_strand)
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
  boost::asio::post(_strand,
                    std::bind(&Music::doPlay, this, streamable));
}

void engine::Music::doPlay(const std::shared_ptr<Streamable> & streamable)
{
  assert(_strand.running_in_this_thread());

  doStream(_buffers[0], streamable);
  doStream(_buffers[1], streamable);

  alSourceQueueBuffers(_source, 2, _buffers);

  alSourcePlay(_source);

  _timer.expires_after(_bufferRefreshInterval);
  _timer.async_wait(std::bind(&Music::doRefresh, this, std::placeholders::_1, streamable));
}

bool engine::Music::doStream(ALuint buffer, const std::shared_ptr<Streamable> & streamable)
{
  assert(_strand.running_in_this_thread());

  _streamBuffer.resize(streamable->getNominalBitrate() * _bufferDuration.count());
  size_t size = streamable->stream(&_streamBuffer[0], _streamBuffer.size());

  if(size > 0)
  {
    alBufferData(buffer,
                 streamable->getNumChannels() == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
                 &_streamBuffer[0],
                 size,
                 streamable->getBitrate());
    return true;
  }
  else
  {
    return false;
  }
}

void engine::Music::doRefresh(const boost::system::error_code & error, const std::shared_ptr<Streamable> & streamable)
{
  assert(_strand.running_in_this_thread());

  if(!error)
  {
    BOOST_LOG_TRIVIAL(info) << "Ready to refresh";
    bool reschedule = true;

    int processed;
    alGetSourcei(_source, AL_BUFFERS_PROCESSED, &processed);

    while(processed--)
    {
      BOOST_LOG_TRIVIAL(info) << "Streaming buffer";
      ALuint buffer;
      alSourceUnqueueBuffers(_source, 1, &buffer);
      reschedule = doStream(buffer, streamable);
      alSourceQueueBuffers(_source, 1, &buffer);
    }

    if(reschedule)
    {
      _timer.expires_after(_bufferRefreshInterval);
      _timer.async_wait(std::bind(&Music::doRefresh, this, std::placeholders::_1, streamable));
    }
  }
}
