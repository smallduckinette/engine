#include "engine/controls/bindings.h"

#include "engine/core/serializejson.h"
#include "engine/core/json_utils.h"

void engine::controls::Mapping::deserialize(const Json::Value & doc)
{
  deserializeJson(_fromMin, doc, "fromMin");
  deserializeJson(_fromMax, doc, "fromMax");
  deserializeJson(_toMin, doc, "toMin");
  deserializeJson(_toMax, doc, "toMax");
}

void engine::controls::Binding::deserialize(const Json::Value & doc)
{
  deserializeJson(_type, doc, "type");
  deserializeJson(_name, doc, "name");
  deserializeJson(_id, doc, "id");
  deserializeJson(_mapping, doc, "mapping");
}

void engine::controls::Device::deserialize(const Json::Value & doc)
{
  deserializeJson(_guid, doc, "guid");
  deserializeJson(_bindings, doc, "bindings");
}

void engine::controls::Controls::deserialize(const Json::Value & doc)
{
  deserializeJson(_devices, doc, "devices");
}
