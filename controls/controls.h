#ifndef __CONTROLS_CONTROLS_H__
#define __CONTROLS_CONTROLS_H__

#include <SDL2/SDL.h>

namespace engine
{
  class Controls
  {
  public:
    Controls();
    
    void process(const SDL_Event & event);
    
  private:
  };
}

#endif
