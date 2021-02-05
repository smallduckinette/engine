#ifndef __ENTITY_ENTITYLIBRARY_H__
#define __ENTITY_ENTITYLIBRARY_H__

#include <vector>
#include <json/json.h>

#include "engine/entity/entityid.h"

namespace engine
{
  class System;

  class EntityLibrary
  {
  public:
    EntityLibrary();

    void registerSystem(System * system);

    void registerArchetypes(const Json::Value & doc);

    EntityId createEntity(const std::string & archetype);

  private:
    EntityIdGenerator _generator;
    std::vector<System *> _systems;
  };
}

#endif
