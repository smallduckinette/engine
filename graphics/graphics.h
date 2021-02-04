#ifndef __GRAPHICS_GRAPHICS_H__
#define __GRAPHICS_GRAPHICS_H__

#include <glm/gtx/rotate_vector.hpp>

#include "engine/entity/system.h"

namespace engine::adh { class Node; }
namespace engine::adh { class Camera; }

namespace engine
{
  class Clock;

  class Graphics : public System
  {
  public:
    Graphics(Clock * clock,
             int resX,
             int resY,
             const std::filesystem::path & dataDir);

    /// Add an environment map
    void addEnvmap(const std::filesystem::path & envMap);

    /// Change the view
    void setView(const glm::mat4 & view);

    /// Add a model
    void addModel(const std::filesystem::path & file);

    /// Render the frame
    void display();

    // Overrides of the System interface
    void init(const std::filesystem::path & rootDirectory) override;

    void add(EntityId entityId,
             const Json::Value & doc) override;

  private:
    Clock * _clock;
    std::filesystem::path _shaderDir;

    std::shared_ptr<adh::Node> _root;
    std::shared_ptr<adh::Camera> _camera;
  };
}

#endif
