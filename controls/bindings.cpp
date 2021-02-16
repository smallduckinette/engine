#include "engine/controls/bindings.h"

#include "engine/core/serializejson.h"
#include "engine/core/json_utils.h"

void engine::controls::Binding::deserialize(const Json::Value & doc)
{
  deserializeJson(_type, doc, "type");
  deserializeJson(_name, doc, "name");
  deserializeJson(_id, doc, "id");
}

void engine::controls::Device::deserialize(const Json::Value & doc)
{
  deserializeJson(_guid, doc, "guid");
  for(auto && node : getNodeOrThrow(doc, "bindings"))
  {
    Binding binding;
    binding.deserialize(node);
    _bindings.push_back(binding);
  }
}

void engine::controls::Controls::deserialize(const Json::Value & doc)
{
  for(auto && node : getNodeOrThrow(doc, "devices"))
  {
    Device device;
    device.deserialize(node);
    _devices.push_back(device);
  }
}
