#ifndef __GRAPHICS_GRAPHICS_H__
#define __GRAPHICS_GRAPHICS_H__

#include <map>
#include <unordered_map>

#include <glm/gtx/rotate_vector.hpp>

#include "engine/entity/system.h"

namespace engine::adh { class Node; }
namespace engine::adh { class Camera; }
namespace engine::adh { class Transform; }

namespace engine
{
  class Clock;

  class GraphicsArchetype
  {
  public:
    void deserialize(const Json::Value & doc);

    auto operator<=>(const GraphicsArchetype &) const = default;

    std::filesystem::path _model;
  };

  class Graphics : public System
  {
  public:
    Graphics(Clock * clock,
             int resX,
             int resY,
             const std::filesystem::path & dataDir);

    /// Register an archetype from json
    void registerArchetype(const std::string & name, const Json::Value & doc) override;

    /// Register an archetype from structure
    void registerArchetype(const std::string & name, const GraphicsArchetype & archetype);

    /// Create the entity from the archetype
    void createEntity(EntityId entityId, const std::string & archetype) override;

    /// Add an environment map
    void addEnvmap(const std::filesystem::path & envMap);

    /// Change the view
    void setView(const glm::mat4 & view);

    /// Render the frame
    void display();

  private:
    Clock * _clock;
    std::filesystem::path _shaderDir;

    std::shared_ptr<adh::Node> _root;
    std::shared_ptr<adh::Camera> _camera;

    /// Archetypes
    std::unordered_map<std::string, GraphicsArchetype> _archetypes;

    /// Entities
    std::map<EntityId, std::shared_ptr<adh::Transform> > _entities;
  };
}

#endif
