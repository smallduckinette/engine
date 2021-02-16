#include "engine/controls/controls.h"

#include <boost/log/trivial.hpp>

#include "engine/core/json_utils.h"
#include "engine/controls/bindings.h"


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

  controls::Controls configControls;
  configControls.deserialize(config);

  for(auto && configDevice : configControls._devices)
  {
    auto it = _guidToIndex.find(configDevice._guid);
    if(it == _guidToIndex.end())
    {
      BOOST_LOG_TRIVIAL(error) << "No device found with GUID " << configDevice._guid;
    }
    else
    {
      DeviceBindings & bindings = _deviceBindings.emplace(it->second, DeviceBindings()).first->second;
      for(auto && configBinding : configDevice._bindings)
      {
        auto itb = userBindings.find(configBinding._name);
        if(itb == userBindings.end())
        {
          BOOST_LOG_TRIVIAL(error) << "Configuration references unknown binding " << configBinding._name;
        }
        else if(configBinding._type == "button")
        {
          bindings._buttons.insert_or_assign(configBinding._id, itb->second);
          BOOST_LOG_TRIVIAL(info) << "Device " << configDevice._guid << "[" << it->second << "] button " << configBinding._id << " assigned to event " << configBinding._name << "[" << itb->second << "]";
        }
        else if(configBinding._type == "axis")
        {
          bindings._axis.insert_or_assign(configBinding._id, itb->second);
          BOOST_LOG_TRIVIAL(info) << "Device " << configDevice._guid << "[" << it->second << "] axis " << configBinding._id << " assigned to event " << configBinding._name << "[" << itb->second << "]";
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
