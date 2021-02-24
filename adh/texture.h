#ifndef __ADH_TEXTURE_H__
#define __ADH_TEXTURE_H__

#include <string>
#include <GL/glew.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

namespace engine::adh
{
  class Texture
  {
  public:
    Texture(const std::string & name);
    Texture(const std::string & name,
            const std::string & filename);
    ~Texture();

    /// Set texture from a surface
    void setTexture(const SDL_Surface * surface);

    /// Set a texture from a text
    void setText(TTF_Font * font,
                 SDL_Color color,
                 const std::string & text);

    void bind(GLenum texture);
    std::string getName() const;

    int getWidth() const;
    int getHeight() const;

  private:
    const std::string _name;
    GLuint _textureId;
    int _width;
    int _height;
  };
}

#endif
