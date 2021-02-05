#include "engine/entity/entitylibrary.h"

#include <boost/log/trivial.hpp>

#include "engine/core/serializejson.h"
#include "engine/entity/system.h"

engine::EntityLibrary::EntityLibrary():
  _generator(1)
{
}

void engine::EntityLibrary::registerSystem(System * system)
{
  _systems.push_back(system);
}

void engine::EntityLibrary::registerArchetypes(const Json::Value & doc)
{
  for(auto && node : doc)
  {
    std::string archetypeName;
    deserializeJson(archetypeName, node, "archetypeName");

    BOOST_LOG_TRIVIAL(info) << "Register archetype " << archetypeName;
    for(auto && system : _systems)
    {
      system->registerArchetype(archetypeName, node);
    }
  }
}

engine::EntityId engine::EntityLibrary::createEntity(const std::string & archetype)
{
  EntityId entityId = _generator.generate();
  for(auto && system : _systems)
  {
    system->createEntity(entityId, archetype);
  }
  return entityId;
}
