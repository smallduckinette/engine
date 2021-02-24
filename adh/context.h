#ifndef __ADH_CONTEXT_H__
#define __ADH_CONTEXT_H__

#include <glm/glm.hpp>

namespace engine::adh
{
  class Context
  {
  public:
    Context(int resX, int resY);

    glm::mat4 _model;
    glm::mat4 _view;
    glm::mat4 _projection;
    glm::vec3 _camPos;

    glm::vec3 _lightPosition;
    glm::vec3 _lightColor;

    int _resX;
    int _resY;
  };
}

#endif
