#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>

engine::adh::Transform::Transform(const std::string & name):
  Node(name),
  _matrix(1.0f),
  _translate(1.0f),
  _rotate(1.0f),
  _scale(1.0f)
{
}

void engine::adh::Transform::draw(Context & context) const
{
  glm::mat4 previousModel = context._model;
  context._model *= _matrix;
  Node::draw(context);
  context._model = previousModel;
}

void engine::adh::Transform::setMatrix(const glm::mat4 & matrix)
{
  _matrix = matrix;
}

void engine::adh::Transform::setTranslate(const glm::vec3 & translate)
{
  _translate = glm::translate(glm::mat4(1.0), translate);
  recomputeMatrix();
}

void engine::adh::Transform::setRotate(const glm::quat & rotate)
{
  _rotate = glm::mat4_cast(rotate);
  recomputeMatrix();
}

void engine::adh::Transform::setScale(const glm::vec3 & scale)
{
  _scale = glm::scale(glm::mat4(1.0), scale);
  recomputeMatrix();
}

void engine::adh::Transform::recomputeMatrix()
{
  _matrix = _translate * _rotate * _scale;
}
