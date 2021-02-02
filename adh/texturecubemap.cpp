#include "engine/adh/texturecubemap.h"

#include <memory>
#include <vector>
#include <SDL2/SDL_image.h>

engine::adh::TextureCubeMap::TextureCubeMap(const std::string & name,
                                            const std::string & filename):
  _name(name)
{
  glGenTextures(1, &_textureId);

  try
  {
    glBindTexture(GL_TEXTURE_CUBE_MAP, _textureId);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>
      imageData(IMG_Load(filename.c_str()),
                SDL_FreeSurface);

    if(imageData->w % 4 != 0)
      throw std::runtime_error("Cubemap width not divisible by 4");

    if(imageData->h % 3 != 0)
      throw std::runtime_error("Cubemap height not divisible by 3");

    if(imageData->w / 4 != imageData->h / 3)
      throw std::runtime_error("Unexpected size of cubemap");

    int cubeSide = imageData->w / 4;
    std::vector<std::pair<int, int> > positions({{2, 1}, {0, 1}, {1, 0}, {1, 2}, {1, 1}, {3, 1}});

    GLenum mode = (imageData->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;

    for(unsigned int index = 0; index < 6; ++index)
    {
      std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>
        texture(SDL_CreateRGBSurface(0,
                                     cubeSide,
                                     cubeSide,
                                     imageData->format->BitsPerPixel,
                                     imageData->format->Rmask,
                                     imageData->format->Gmask,
                                     imageData->format->Bmask,
                                     imageData->format->Amask),
                SDL_FreeSurface);

      auto position = positions.at(index);
      SDL_Rect sourceRect = {cubeSide * position.first, cubeSide* position.second,
                             cubeSide * (position.first + 1), cubeSide * (position.second + 1)};

      SDL_BlitSurface(imageData.get(), &sourceRect, texture.get(), nullptr);

      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index,
                   0,
                   mode,
                   cubeSide,
                   cubeSide,
                   0,
                   mode,
                   GL_UNSIGNED_BYTE,
                   texture->pixels);
    }
  }
  catch(const std::exception &)
  {
    glDeleteTextures(1, &_textureId);
    throw;
  }
}

engine::adh::TextureCubeMap::~TextureCubeMap()
{
  glDeleteTextures(1, &_textureId);
}

std::string engine::adh::TextureCubeMap::getName() const
{
  return _name;
}

void engine::adh::TextureCubeMap::bind(GLenum)
{
  //glActiveTexture(texture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, _textureId);
}
