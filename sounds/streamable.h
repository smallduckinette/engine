#ifndef __SOUNDS_STREAMABLE_H__
#define __SOUNDS_STREAMABLE_H__

#include <filesystem>
#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>


namespace engine
{
  /// Interface for streamable music
  class Streamable
  {
  public:
    virtual ~Streamable() = default;

    /// Load the next buffer of data
    /// @returns Amount of data read in the buffer. If this is
    /// lower than bufferSize, then we know we have reached the end
    /// @throws std::exception when failing to read the data
    virtual size_t stream(char * buffer, size_t bufferSize) = 0;
  };

  /// Vorbis implementation of the Streamable interface
  class VorbisStreamable : public Streamable
  {
  public:
    VorbisStreamable(const std::filesystem::path & file);

    /// Implementation of the Streamable interface
    size_t stream(char * buffer, size_t bufferSize) override;

  private:
    std::unique_ptr<FILE, decltype(&fclose)> _file;
    OggVorbis_File _vorbisFile;
    std::unique_ptr<OggVorbis_File, decltype(&ov_clear)> _vorbisFilePtr;
    vorbis_info * _vorbisInfo;
    vorbis_comment * _vorbisComment;
  };
}

#endif
