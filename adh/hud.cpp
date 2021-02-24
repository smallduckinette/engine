#include "engine/adh/hud.h"

#include "engine/adh/texture.h"
#include "engine/adh/shader.h"

engine::adh::Hud::Hud(const std::shared_ptr<Texture> & texture,
                      const std::shared_ptr<Shader> & shader):
  _texture(texture),
  _shader(shader)
{
}

void engine::adh::Hud::draw(Context & context) const
{
  _texture->bind(GL_TEXTURE0);
  _shader->use();
  _shader->setInteger(_texture->getName(), 0);
  _shader->setVector("range", glm::vec3(float(_texture->getWidth()) / context._resX,
                                        float(_texture->getHeight()) / context._resY,
                                        1));

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glDisable(GL_BLEND);

  Node::draw(context);
}
