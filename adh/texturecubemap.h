#ifndef __ADH_TEXTURECUBEMAP_H__
#define __ADH_TEXTURECUBEMAP_H__

#include <string>
#include <GL/glew.h>

namespace engine::adh
{
  class TextureCubeMap
  {
  public:
    TextureCubeMap(const std::string & name,
                   const std::string & filename);
    ~TextureCubeMap();

    void bind(GLenum texture);
    std::string getName() const;

  private:
    std::string _name;
    GLuint _textureId;
  };
}

#endif
