#ifndef __SOUNDS_MUSIC_H__
#define __SOUNDS_MUSIC_H__

#include <memory>
#include <vector>

#include <AL/al.h>

namespace engine
{
  class Streamable;

  class Music
  {
  public:
    Music();
    ~Music();

    void play(const std::shared_ptr<Streamable> & streamable);

  private:
    void stream(ALuint buffer, const std::shared_ptr<Streamable> & streamable);

    ALuint _source;
    ALuint _buffers[2];
    std::vector<char> _streamBuffer;
  };
}

#endif
