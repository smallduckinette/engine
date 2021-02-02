#include "engine/adh/envmap.h"

#include "engine/adh/texturecubemap.h"
#include "engine/adh/shader.h"

engine::adh::EnvMap::EnvMap(const std::shared_ptr<TextureCubeMap> & texture,
                            const std::shared_ptr<Shader> & shader):
  _texture(texture),
  _shader(shader)
{
}

engine::adh::EnvMap::~EnvMap()
{
}

void engine::adh::EnvMap::draw(Context & context) const
{
  _texture->bind(GL_TEXTURE0);
  _shader->setInteger(_texture->getName(), 0);
  _shader->use();
  _shader->setMatrix("viewMatrix", context._view);
  _shader->setMatrix("projectionMatrix", context._projection);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  Node::draw(context);
}
