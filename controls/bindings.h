#ifndef __CONTROLS_BINDINGS_H__
#define __CONTROLS_BINDINGS_H__

#include <json/json.h>

namespace engine
{
  namespace controls
  {
    class Binding
    {
    public:
      void deserialize(const Json::Value & doc);

      auto operator<=>(const Binding &) const = default;

      std::string _type;
      std::string _name;
      int _id;
    };

    class Device
    {
    public:
      void deserialize(const Json::Value & doc);

      auto operator<=>(const Device &) const = default;

      std::string _guid;
      std::vector<Binding> _bindings;
    };

    class Controls
    {
    public:
      void deserialize(const Json::Value & doc);

      auto operator<=>(const Controls &) const = default;

      std::vector<Device> _devices;
    };
  }
}

#endif
