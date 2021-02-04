#include "engine/graphics/graphics.h"

#include <fstream>

#include <boost/log/trivial.hpp>
#include <GL/glew.h>

#include "engine/adh/transform.h"
#include "engine/adh/camera.h"
#include "engine/adh/envmap.h"
#include "engine/adh/texturecubemap.h"
#include "engine/adh/shader.h"
#include "engine/adh/animation.h"
#include "engine/gltf/builder.h"


engine::Graphics::Graphics(Clock * clock,
                           int resX,
                           int resY,
                           const std::filesystem::path & dataDir):
  _clock(clock),
  _shaderDir(dataDir / "shaders")
{
  if(!std::filesystem::exists(_shaderDir))
  {
    throw std::runtime_error("Cannot find shader directory " + _shaderDir.native());
  }

  glewExperimental = GL_TRUE;
  glewInit();

  glViewport(0, 0, resX, resY);

  glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString(GL_VERSION); // version as a string

  BOOST_LOG_TRIVIAL(info) << "Renderer: " << renderer;
  BOOST_LOG_TRIVIAL(info) << "Version: " << version;

  _root = std::make_shared<engine::adh::Transform>();
  _camera = std::make_shared<engine::adh::Camera>(glm::radians(45.0f),
                                                  (float)resX / (float)resY,
                                                  0.1f,
                                                  10000.0f);
  _camera->addChild(_root);
}

void engine::Graphics::addEnvmap(const std::filesystem::path & envMap)
{
  std::ifstream vertex(_shaderDir / "envmap.vert");
  std::ifstream fragment(_shaderDir / "envmap.frag");
  _camera->addChild(std::make_shared<engine::adh::EnvMap>
                    (std::make_shared<engine::adh::TextureCubeMap>("texture", envMap),
                     std::make_shared<engine::adh::Shader>(vertex, fragment)));
}

void engine::Graphics::setView(const glm::mat4 & view)
{
  _camera->setViewMatrix(view);
}

void engine::Graphics::addModel(const std::filesystem::path & file)
{
  gltf::Builder builder(_clock,
                        _shaderDir,
                        file);

  std::vector<std::unique_ptr<engine::adh::Animation> > animations;
  _root->addChild(builder.build(animations));
}

void engine::Graphics::display()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  engine::adh::Context context;
  context._lightPosition = glm::vec3(-5.0f, 0.0f, 0.0f);
  _camera->draw(context);
}

void engine::Graphics::init(const std::filesystem::path & )
{
}

void engine::Graphics::add(EntityId ,
                           const Json::Value & )
{
}
