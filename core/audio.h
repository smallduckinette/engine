#ifndef __CORE_AUDIO_H__
#define __CORE_AUDIO_H__

#include <string>

#include <AL/al.h>
#include <AL/alc.h>

namespace engine
{
  std::string getAlcErrorString();
}

#endif
