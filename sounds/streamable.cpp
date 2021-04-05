#include "engine/sounds/streamable.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>

engine::VorbisStreamable::VorbisStreamable(const std::filesystem::path & file):
  _file(fopen(file.c_str(), "rb"), &fclose),
  _vorbisFilePtr(nullptr, &ov_clear)
{
  if(ov_open_callbacks(_file.get(), &_vorbisFile, nullptr, 0, OV_CALLBACKS_NOCLOSE) != 0)
    throw std::runtime_error("Failed to open Vorbis file" + file.native());
  _vorbisFilePtr.reset(&_vorbisFile);

  _vorbisInfo = ov_info(&_vorbisFile, -1);
  _vorbisComment = ov_comment(&_vorbisFile, -1);

  BOOST_LOG_TRIVIAL(debug) << "Ogg/Vorbis" << file << " properties:";
  BOOST_LOG_TRIVIAL(debug) << "version         " << _vorbisInfo->version;
  BOOST_LOG_TRIVIAL(debug) << "channels        " << _vorbisInfo->channels;
  BOOST_LOG_TRIVIAL(debug) << "rate (hz)       " << _vorbisInfo->rate;
  BOOST_LOG_TRIVIAL(debug) << "bitrate upper   " << _vorbisInfo->bitrate_upper;
  BOOST_LOG_TRIVIAL(debug) << "bitrate nominal " << _vorbisInfo->bitrate_nominal;
  BOOST_LOG_TRIVIAL(debug) << "bitrate lower   " << _vorbisInfo->bitrate_lower;
  BOOST_LOG_TRIVIAL(debug) << "bitrate window  " << _vorbisInfo->bitrate_window;
  BOOST_LOG_TRIVIAL(debug) << "vendor " << _vorbisComment->vendor;
}

size_t engine::VorbisStreamable::stream(char * buffer, size_t bufferSize)
{
  int section = 0;
  size_t totalRead = 0;

  while(bufferSize > 0)
  {
    int result = ov_read(&_vorbisFile, buffer, bufferSize, 0, 2, 1, &section);
    if(result == 0)
    {
      break;
    }
    else if(result >= 0)
    {
      bufferSize -= result;
      buffer += result;
      totalRead += result;
    }
    else if(result == OV_HOLE)
      throw std::runtime_error("Data interruption");
    else if(result == OV_EBADLINK)
      throw std::runtime_error("Invalid stream section");
    else if(result == OV_EINVAL)
      throw std::runtime_error("Invalid file headers");
    else
      throw std::runtime_error("Unknown error");
  }
  return totalRead;
}

int engine::VorbisStreamable::getBitrate() const
{
  return _vorbisInfo->rate;
}

int engine::VorbisStreamable::getNominalBitrate() const
{
  return _vorbisInfo->bitrate_nominal;
}

int engine::VorbisStreamable::getNumChannels() const
{
  return _vorbisInfo->channels;
}
