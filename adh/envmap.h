#ifndef __ADH_ENVMAP_H__
#define __ADH_ENVMAP_H__

#include <GL/glew.h>

#include "engine/adh/node.h"

namespace engine::adh
{
  class Shader;
  class Texture;

  class EnvMap : public Node
  {
  public:
    EnvMap(const std::shared_ptr<Texture> & texture,
           const std::shared_ptr<Shader> & shader);
    ~EnvMap();

    void draw(Context & context) const override;

  private:
    const std::shared_ptr<Texture> _texture;
    const std::shared_ptr<Shader> _shader;
  };
}

#endif
