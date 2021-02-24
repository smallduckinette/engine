#include "context.h"

#include <glm/gtc/matrix_transform.hpp>

engine::adh::Context::Context(int resX, int resY):
  _model(1.0f),
  _view(1.0f),
  _projection(1.0f),
  _camPos(glm::vec3(0.0f, 0.0f, -1.0f)),
  _lightPosition(0.0f, 0.0f, -5.0f),
  _lightColor(300.0f, 300.0f, 300.0f),
  _resX(resX),
  _resY(resY)
{
}
