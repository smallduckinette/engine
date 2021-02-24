#ifndef __ADH_HUD_H__
#define __ADH_HUD_H__

#include "engine/adh/node.h"

namespace engine::adh
{
  class Texture;
  class Shader;

  /// A flat, untransformed quad with a texture
  class Hud : public Node
  {
  public:
    Hud(const std::shared_ptr<Texture> & texture,
        const std::shared_ptr<Shader> & shader);

    void draw(Context & context) const override;

  private:
    const std::shared_ptr<Texture> _texture;
    const std::shared_ptr<Shader> _shader;
  };
}

#endif
