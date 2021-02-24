#ifndef __ADH_ENVMAP_H__
#define __ADH_ENVMAP_H__

#include <GL/glew.h>

#include "engine/adh/node.h"

namespace engine::adh
{
  class Shader;
  class TextureCubeMap;

  class EnvMap : public Node
  {
  public:
    EnvMap(const std::shared_ptr<TextureCubeMap> & texture,
           const std::shared_ptr<Shader> & shader);

    void draw(Context & context) const override;

  private:
    const std::shared_ptr<TextureCubeMap> _texture;
    const std::shared_ptr<Shader> _shader;
  };
}

#endif
