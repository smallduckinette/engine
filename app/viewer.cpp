#include <iostream>

#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "engine/core/app.h"
#include "engine/adh/primitive.h"
#include "engine/adh/camera.h"
#include "engine/adh/shader.h"
#include "engine/adh/transform.h"
#include "engine/adh/rtclock.h"
#include "engine/adh/animation.h"
#include "engine/gltf/builder.h"


namespace po = boost::program_options;

int main(int argc, char ** argv)
{
  try
  {
    int resX = 1280;
    int resY = 1024;
    std::string model;
    std::filesystem::path dataDir;

    engine::App app;

    app.add_options()
      ("x", po::value<int>(&resX)->default_value(resX), "X resolution")
      ("y", po::value<int>(&resY)->default_value(resY), "Y resolution")
      ("model,m", po::value<std::string>(&model)->required(), "model to display")
      ("data,d", po::value<std::filesystem::path>(&dataDir)->default_value("data"), "data directory")
      ("full-screen", "Full screen")
      ("vsync", "Enable vertical synchronization");

    if(app.run(argc, argv))
    {
      BOOST_LOG_TRIVIAL(info) << "Init SDL";
      if(SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error(std::string("Failed to init SDL : ") + SDL_GetError());

      unsigned int videoFlags = SDL_WINDOW_OPENGL;
      if(app.has("full-screen"))
      {
        SDL_DisplayMode displayMode;
        if(SDL_GetDesktopDisplayMode(0, &displayMode) < 0)
          throw std::runtime_error(std::string("Failed to get display mode : ") + SDL_GetError());
        resX = displayMode.w;
        resY = displayMode.h;
        videoFlags |= SDL_WINDOW_FULLSCREEN;
      }

      BOOST_LOG_TRIVIAL(info) << "Setting up video mode to " << resX << "x" << resY;

      SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

      std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>
        window(SDL_CreateWindow("Viewer",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                resX,
                                resY,
                                videoFlags),
               SDL_DestroyWindow);

      std::unique_ptr<void, decltype(&SDL_GL_DeleteContext)>
        glContext(SDL_GL_CreateContext(window.get()),
                  SDL_GL_DeleteContext);

      if(app.has("vsync"))
        SDL_GL_SetSwapInterval(1);

      glewExperimental = GL_TRUE;
      glewInit();

      glViewport(0, 0, resX, resY);

      glEnable(GL_MULTISAMPLE);
      glEnable(GL_DEPTH_TEST);

      const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
      const GLubyte* version = glGetString(GL_VERSION); // version as a string

      BOOST_LOG_TRIVIAL(info) << "Renderer: " << renderer;
      BOOST_LOG_TRIVIAL(info) << "Version: " << version;

      auto clock = std::make_shared<engine::adh::RtClock>();

      auto shaderDir = dataDir / "shaders";
      engine::gltf::Builder builder(clock, shaderDir, model);
      auto transform = std::make_shared<engine::adh::Transform>();
      auto camera = std::make_shared<engine::adh::Camera>(glm::radians(45.0f),
                                                          (float)resX / (float)resY,
                                                          0.1f,
                                                          100.0f);
      std::vector<std::unique_ptr<engine::adh::Animation> > animations;
      transform->addChild(builder.build(animations));
      camera->addChild(transform);

      camera->setViewMatrix(glm::lookAt(glm::vec3(-5.0f, 0.0f, 0.0f),
                                        glm::vec3(0.0f, 0.0f, 0.0f),
                                        glm::vec3(0.0f, 0.0f, 1.0f)));

      bool running = true;
      auto t1 = std::chrono::steady_clock::now();
      while(running)
      {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
          if(event.type == SDL_QUIT)
          {
            running = false;
          }
        }

        auto t2 = std::chrono::steady_clock::now();
        std::chrono::duration<float> d = (t2 - t1);
        BOOST_LOG_TRIVIAL(debug) << 1 / d.count();
        t1 = t2;

        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        engine::adh::Context context;
        context._lightPosition = glm::vec3(0.0f, -5.0f, 0.0f);
        camera->draw(context);

        SDL_GL_SwapWindow(window.get());
      }
    }

    SDL_Quit();
    return 0;
  }
  catch(const std::exception & e)
  {
    SDL_Quit();
    BOOST_LOG_TRIVIAL(error) << e.what();
    return -1;
  }
}
