#ifndef __SOUNDS_SOUNDS_H__
#define __SOUNDS_SOUNDS_H__

#include "engine/entity/system.h"

namespace engine
{
  class SoundsArchetype
  {
  public:
    void deserialize(const Json::Value & doc);

    auto operator<=>(const SoundsArchetype &) const = default;

    std::filesystem::path _soundFile;
  };

  /// The sounds system
  class Sounds : public System
  {
  public:
    Sounds();

    /// Register an archetype from json
    void registerArchetype(const std::string & name, const Json::Value & doc) override;

    /// Register an archetype from structure
    void registerArchetype(const std::string & name, const SoundsArchetype & archetype);

    /// Create the entity from the archetype
    void createEntity(EntityId entityId, const std::string & archetype) override;
  };
}

#endif
