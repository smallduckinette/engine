#include "engine/controls/controls.h"

#include <boost/log/trivial.hpp>

#include "engine/core/json_utils.h"


engine::Controls::Controls()
{
  initSDL();
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

engine::Signal<engine::Controls::BindingId> & engine::Controls::onButton()
{
  return _buttonSignal;
}

engine::Signal<engine::Controls::BindingId, int> & engine::Controls::onAxis()
{
  return _axisSignal;
}

void engine::Controls::initSDL()
{
  SDL_InitSubSystem(SDL_INIT_JOYSTICK);

  for(int joyIndex = 0; joyIndex < SDL_NumJoysticks(); ++joyIndex)
  {
    SDL_Joystick * joy = SDL_JoystickOpen(joyIndex);
    if(joy)
    {
      char guid[33];
      SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(joy), guid, 33);
      _guidToIndex.emplace(std::string(guid), joyIndex);

      BOOST_LOG_TRIVIAL(debug) << "Controller[" << joyIndex << "] " << SDL_JoystickName(joy) << " (" << guid << ")";
      BOOST_LOG_TRIVIAL(debug) << "Controller[" << joyIndex << "] Number of axis: " << SDL_JoystickNumAxes(joy);
      BOOST_LOG_TRIVIAL(debug) << "Controller[" << joyIndex << "] Number of buttons: " << SDL_JoystickNumButtons(joy);
      BOOST_LOG_TRIVIAL(debug) << "Controller[" << joyIndex << "] Number of balls: " << SDL_JoystickNumBalls(joy);
      BOOST_LOG_TRIVIAL(debug) << "Controller[" << joyIndex << "] Number of hats: " << SDL_JoystickNumHats(joy);
    }
    else
    {
      BOOST_LOG_TRIVIAL(debug) << "Failed to open controller " << joyIndex;
    }
  }
}

void engine::Controls::initConfig(const Json::Value & config,
                                  const std::map<std::string, BindingId> & userBindings)
{
  BOOST_LOG_TRIVIAL(debug) << "Reading configuration";

  if(config != Json::Value())
  {
    for(auto && device : getNodeOrThrow(config, "devices"))
    {
      std::string guid;
      get(device, "guid", guid);
      auto it = _guidToIndex.find(guid);
      if(it == _guidToIndex.end())
      {
        BOOST_LOG_TRIVIAL(error) << "No device found with GUID " << guid;
      }
      else
      {
        DeviceBindings & bindings = _deviceBindings.emplace(it->second, DeviceBindings()).first->second;

        for(auto && binding : getNodeOrThrow(device, "bindings"))
        {
          std::string type;
          get(binding, "type", type);
          std::string name;
          get(binding, "name", name);
          unsigned int id;
          get(binding, "id", id);

          auto itb = userBindings.find(name);
          if(itb == userBindings.end())
          {
            BOOST_LOG_TRIVIAL(error) << "Configuration references unknown binding " << name;
          }
          else if(type == "button")
          {
            bindings._buttons.insert_or_assign(id, itb->second);
            BOOST_LOG_TRIVIAL(info) << "Device " << guid << "[" << it->second << "] button " << id << " assigned to event " << name << "[" << itb->second << "]";
          }
          else if(type == "axis")
          {
            bindings._axis.insert_or_assign(id, itb->second);
            BOOST_LOG_TRIVIAL(info) << "Device " << guid << "[" << it->second << "] axis " << id << " assigned to event " << name << "[" << itb->second << "]";
          }
        }
      }
    }
  }
}

void engine::Controls::onButton(const SDL_JoyButtonEvent & event)
{
  BOOST_LOG_TRIVIAL(debug)
    << "Controller[" << event.which << "] Button " << static_cast<unsigned int>(event.button) << " "
    << (event.type == SDL_JOYBUTTONDOWN ? "SDL_JOYBUTTONDOWN" : "SDL_JOYBUTTONUP")
    << " "
    << (event.state == SDL_PRESSED ? "SDL_PRESSED" : "SDL_RELEASED")
    << " at " << event.timestamp;

  auto it_controller = _deviceBindings.find(event.which);
  if(it_controller != _deviceBindings.end())
  {
    const DeviceBindings & bindings = it_controller->second;

    auto it_button = bindings._buttons.find(event.button);
    if(it_button != bindings._buttons.end())
    {
      BindingId bindingID = it_button->second;
      if(event.type == SDL_JOYBUTTONDOWN)
      {
        _buttonSignal.emit(bindingID);
      }
    }
  }
}

void engine::Controls::onAxis(const SDL_JoyAxisEvent & event)
{
  BOOST_LOG_TRIVIAL(debug)
    << "Controller[" << event.which << "] Axis " << static_cast<unsigned int>(event.axis) << " "
    << " "
    << event.value
    << " at " << event.timestamp;

  auto it_controller = _deviceBindings.find(event.which);
  if(it_controller != _deviceBindings.end())
  {
    const DeviceBindings & bindings = it_controller->second;

    auto it_axis = bindings._axis.find(event.axis);
    if(it_axis != bindings._axis.end())
    {
      BindingId bindingID = it_axis->second;
      _axisSignal.emit(bindingID, event.value);
    }
  }
}

void engine::Controls::onHat(const SDL_JoyHatEvent & event)
{
  BOOST_LOG_TRIVIAL(debug)
    << "Controller[" << event.which << "] Hat " << static_cast<unsigned int>(event.hat) << " "
    << " "
    << static_cast<unsigned int>(event.value)
    << " at " << event.timestamp;
}
