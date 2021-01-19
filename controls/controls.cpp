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

void engine::Controls::process(const SDL_Event & event)
{
  switch(event.type)
  {
  case SDL_JOYBUTTONDOWN:
  case SDL_JOYBUTTONUP:
    onButton(event.jbutton);
    break;
  case SDL_JOYAXISMOTION:
    onAxis(event.jaxis);
    break;
  case SDL_JOYHATMOTION:
    onHat(event.jhat);
    break;
  default:
    break;
  }
}

void engine::Controls::onButton(const SDL_JoyButtonEvent & event)
{
  BOOST_LOG_TRIVIAL(info)
    << "Controller[" << event.which << "] Button " << static_cast<unsigned int>(event.button) << " "
    << (event.type == SDL_JOYBUTTONDOWN ? "SDL_JOYBUTTONDOWN" : "SDL_JOYBUTTONUP")
    << " "
    << (event.state == SDL_PRESSED ? "SDL_PRESSED" : "SDL_RELEASED")
    << " at " << event.timestamp;
}

void engine::Controls::onAxis(const SDL_JoyAxisEvent & event)
{
  BOOST_LOG_TRIVIAL(info)
    << "Controller[" << event.which << "] Axis " << static_cast<unsigned int>(event.axis) << " "
    << " "
    << event.value
    << " at " << event.timestamp;
}

void engine::Controls::onHat(const SDL_JoyHatEvent & event)
{
  BOOST_LOG_TRIVIAL(info)
    << "Controller[" << event.which << "] Hat " << static_cast<unsigned int>(event.hat) << " "
    << " "
    << static_cast<unsigned int>(event.value)
    << " at " << event.timestamp;
}
