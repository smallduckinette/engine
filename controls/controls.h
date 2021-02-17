#ifndef __CONTROLS_CONTROLS_H__
#define __CONTROLS_CONTROLS_H__

#include <SDL2/SDL.h>
#include <json/json.h>
#include <unordered_map>

#include "engine/entity/strongid.h"
#include "engine/entity/signal.h"
#include "engine/controls/mapping.h"

namespace engine
{
  class Controls
  {
  public:
    class BindingDiscriminant;
    using BindingId = StrongId<BindingDiscriminant, unsigned int>;

    using AxisMapping = Mapping<int16_t, float>;

    Controls();

    /// Init controls
    void initConfig(const Json::Value & config,
                    const std::map<std::string, BindingId> & userBindings);

    /// Process events
    void process(const SDL_Event & event);

    /// Signal triggered when a button is pushed
    Signal<BindingId> & onButton();

    /// Signal triggered when an axis is moved
    Signal<BindingId, float> & onAxis();

  private:
    struct AxisBinding
    {
      BindingId _bindingId;
      AxisMapping _mapping;
    };

    struct DeviceBindings
    {
      std::unordered_map<unsigned int, BindingId> _buttons;
      std::unordered_map<unsigned int, AxisBinding> _axis;
    };

    void initSDL();

    void onButton(const SDL_JoyButtonEvent & event);
    void onAxis(const SDL_JoyAxisEvent & event);
    void onHat(const SDL_JoyHatEvent & event);

    std::map<std::string, unsigned int> _guidToIndex;
    std::unordered_map<unsigned int, DeviceBindings> _deviceBindings;

    Signal<BindingId> _buttonSignal;
    Signal<BindingId, float> _axisSignal;
  };
}

#endif
