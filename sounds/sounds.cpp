#include "engine/sounds/sounds.h"

#include "engine/core/serializejson.h"

void engine::SoundsArchetype::deserialize(const Json::Value & doc)
{
  deserializeJson(_soundFile, doc, "soundFile");
}

void engine::Sounds::registerArchetype(const std::string & , const Json::Value & )
{
}

void engine::Sounds::registerArchetype(const std::string & , const SoundsArchetype & )
{
}

void engine::Sounds::createEntity(EntityId , const std::string & )
{
}
