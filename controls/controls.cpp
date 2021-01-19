#include "engine/controls/controls.h"

#include <boost/log/trivial.hpp>


engine::Controls::Controls()
{
  SDL_InitSubSystem(SDL_INIT_JOYSTICK);

  for(int joyIndex = 0; joyIndex < SDL_NumJoysticks(); ++joyIndex)
  {
    SDL_Joystick * joy = SDL_JoystickOpen(joyIndex);
    if(joy)
    {
      char guid[33];
      SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(joy), guid, 33);
      
      BOOST_LOG_TRIVIAL(info) << "Controller[" << joyIndex << "] " << SDL_JoystickName(joy) << " (" << guid << ")";
      BOOST_LOG_TRIVIAL(info) << "Controller[" << joyIndex << "] Number of axis: " << SDL_JoystickNumAxes(joy);
      BOOST_LOG_TRIVIAL(info) << "Controller[" << joyIndex << "] Number of buttons: " << SDL_JoystickNumButtons(joy);
      BOOST_LOG_TRIVIAL(info) << "Controller[" << joyIndex << "] Number of balls: " << SDL_JoystickNumBalls(joy);
      BOOST_LOG_TRIVIAL(info) << "Controller[" << joyIndex << "] Number of hats: " << SDL_JoystickNumHats(joy);
    }
    else
    {
      BOOST_LOG_TRIVIAL(info) << "Failed to open controller " << joyIndex;
    }
  }
}

void engine::Controls::process(const SDL_Event & )
{  
}
