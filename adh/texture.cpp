#include "engine/adh/texture.h"

#include <memory>

engine::adh::Texture::Texture(const std::string & name):
  _name(name),
  _width(0),
  _height(0)
{
  glGenTextures(1, &_textureId);
  glBindTexture(GL_TEXTURE_2D, _textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

engine::adh::Texture::Texture(const std::string & name,
                              const std::string & filename):
  Texture(name)
{
  std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>
    imageData(IMG_Load(filename.c_str()),
              SDL_FreeSurface);
  setTexture(imageData.get());
}

engine::adh::Texture::~Texture()
{
  glDeleteTextures(1, &_textureId);
}

void engine::adh::Texture::setTexture(const SDL_Surface * surface)
{
  GLenum mode = (surface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;

  glBindTexture(GL_TEXTURE_2D, _textureId);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               mode,
               surface->w,
               surface->h,
               0,
               mode,
               GL_UNSIGNED_BYTE,
               surface->pixels);
  glGenerateMipmap(GL_TEXTURE_2D);

  _width = surface->w;
  _height = surface->h;
}

void engine::adh::Texture::setText(TTF_Font * font,
                                   SDL_Color color,
                                   const std::string & text)
{
  std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>
    hudData(SDL_CreateRGBSurfaceWithFormat(0, 256, 256, 32, SDL_PIXELFORMAT_RGBA32),
            SDL_FreeSurface);
  SDL_FillRect(hudData.get(), nullptr, SDL_MapRGBA(hudData->format, 0, 0, 0, 0));

  if(!text.empty())
  {
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>
      imageData(TTF_RenderUTF8_Blended(font, text.c_str(), color),
                SDL_FreeSurface);
    SDL_BlitSurface(imageData.get(), nullptr, hudData.get(), nullptr);

  }
  setTexture(hudData.get());
}

std::string engine::adh::Texture::getName() const
{
  return _name;
}

int engine::adh::Texture::getWidth() const
{
  return _width;
}

int engine::adh::Texture::getHeight() const
{
  return _height;
}

void engine::adh::Texture::bind(GLenum texture)
{
  glActiveTexture(texture);
  glBindTexture(GL_TEXTURE_2D, _textureId);
}
