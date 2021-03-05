#include "engine/core/audio.h"

std::string engine::getAlcErrorString()
{
  ALenum error = alGetError();

  switch(error)
  {
  case AL_NO_ERROR:
    return "No error";
    break;
  case AL_INVALID_NAME:
    return "Invalid name";
    break;
  case AL_INVALID_ENUM:
    return "Invalid enum";
    break;
  case AL_INVALID_VALUE:
    return "Invalid value";
    break;
  case AL_INVALID_OPERATION:
    return "Invalid operation";
    break;
  case AL_OUT_OF_MEMORY:
    return "Out of memory";
    break;
  default:
    return "Unknown error";
  }
}
