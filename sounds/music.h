#ifndef __SOUNDS_MUSIC_H__
#define __SOUNDS_MUSIC_H__

#include <memory>
#include <vector>
#include <chrono>

#include <boost/asio.hpp>
#include <AL/al.h>

namespace engine
{
  class Streamable;

  class Music
  {
  public:
    Music(boost::asio::io_context * ioContext);
    ~Music();

    void play(const std::shared_ptr<Streamable> & streamable);

  private:
    void doPlay(const std::shared_ptr<Streamable> & streamable);
    bool doStream(ALuint buffer, const std::shared_ptr<Streamable> & streamable);
    void doRefresh(const boost::system::error_code & error, const std::shared_ptr<Streamable> & streamable);

    boost::asio::io_context * _ioContext;
    boost::asio::strand<boost::asio::io_context::executor_type> _strand;
    ALuint _source;
    ALuint _buffers[2];
    std::vector<char> _streamBuffer;
    const std::chrono::seconds _bufferDuration;
    const std::chrono::milliseconds _bufferRefreshInterval;
    boost::asio::steady_timer _timer;
  };
}

#endif
