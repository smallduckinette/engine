#ifndef __ENTITY_SYSTEM_H__
#define __ENTITY_SYSTEM_H__

#include <string>
#include <filesystem>
#include <json/json.h>
#include "engine/entity/entityid.h"

namespace engine
{
  class System
  {
  public:
    virtual ~System() = default;

    /// Register an archetype from its Json definition
    virtual void registerArchetype(const std::string & archetype, const Json::Value & doc) = 0;

    /// Create an entity from an archetype
    virtual void createEntity(EntityId entityId, const std::string & archetype) = 0;
  };
}

#endif
