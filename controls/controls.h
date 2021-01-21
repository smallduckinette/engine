#ifndef __CONTROLS_CONTROLS_H__
#define __CONTROLS_CONTROLS_H__

#include <SDL2/SDL.h>
#include <json/json.h>
#include <unordered_map>

#include "engine/entity/strongid.h"
#include "engine/entity/signal.h"

namespace engine
{
  class Controls
  {
  public:
    class BindingDiscriminant;
    using BindingID = StrongId<BindingDiscriminant, unsigned int>;

    Controls(const Json::Value & config,
             const std::map<std::string, BindingID> & userBindings);

    void process(const SDL_Event & event);

    /// Signal triggered when a button is pushed
    Signal<BindingID> & onButton();

    /// Signal triggered when an axis is moved
    Signal<BindingID, int> & onAxis();

  private:
    struct DeviceBindings
    {
      std::unordered_map<unsigned int, BindingID> _buttons;
      std::unordered_map<unsigned int, BindingID> _axis;
    };

    void initSDL();
    void initConfig(const Json::Value & config,
                    const std::map<std::string, BindingID> & userBindings);

    void onButton(const SDL_JoyButtonEvent & event);
    void onAxis(const SDL_JoyAxisEvent & event);
    void onHat(const SDL_JoyHatEvent & event);

    std::map<std::string, unsigned int> _guidToIndex;
    std::unordered_map<unsigned int, DeviceBindings> _deviceBindings;

    Signal<BindingID> _buttonSignal;
    Signal<BindingID, int> _axisSignal;
  };
}

#endif
