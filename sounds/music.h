#ifndef __SOUNDS_MUSIC_H__
#define __SOUNDS_MUSIC_H__

namespace engine
{
  class Music
  {
  public:
    Music(const std::string &

  private:
    FILE * _oggFile;
    OggVorbis_File _oggStream;
    vorbis_info * _vorbisInfo;
    vorbis_comment * _vorbisComment;
  };
}

#endif
