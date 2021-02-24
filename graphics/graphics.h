#ifndef __GRAPHICS_GRAPHICS_H__
#define __GRAPHICS_GRAPHICS_H__

#include <map>
#include <unordered_map>
#include <glm/glm.hpp>
#include <SDL2/SDL_ttf.h>

#include "engine/entity/system.h"

namespace engine::adh { class Node; }
namespace engine::adh { class Camera; }
namespace engine::adh { class Transform; }
namespace engine::adh { class Texture; }

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

    /// Set the transform for an entity
    void setEntityTransform(EntityId entityId, const glm::mat4 & matrix);

    /// Enable the FPS
    void displayFps(bool fps);

    /// Render the frame
    void display();

  private:
    /// Setup the console
    void setupConsole();

    Clock * _clock;
    std::filesystem::path _shaderDir;
    std::filesystem::path _dataDir;

    std::shared_ptr<adh::Node> _root;
    std::shared_ptr<adh::Node> _hudRoot;
    std::shared_ptr<adh::Camera> _camera;

    /// Archetypes
    std::unordered_map<std::string, std::shared_ptr<adh::Node> > _archetypes;

    /// Entities
    std::unordered_map<EntityId, std::shared_ptr<adh::Transform> > _entities;

    /// Resolution
    int _resX;
    int _resY;

    /// Basic HUD font
    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> _font;

    /// Console texture
    std::shared_ptr<adh::Texture> _console;

    /// Do we want to display FPS?
    bool _fpsEnabled;

    /// Frame number for statistics
    int _frameNumber;

    /// Timestamp of first frame for statistics
    float _timestamp;
  };
}

#endif
