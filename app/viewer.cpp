#include <iostream>
#include <fstream>

#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>
#include <SDL2/SDL.h>
#include <glm/gtx/rotate_vector.hpp>

#include "engine/core/app.h"
#include "engine/core/clock.h"
#include "engine/graphics/graphics.h"


namespace po = boost::program_options;

int main(int argc, char ** argv)
{
  try
  {
    std::string model;
    std::filesystem::path dataDir;
    std::filesystem::path envMap;

    engine::App app;

    app.enableGraphics();

    app.add_options()
      ("model,m", po::value<std::string>(&model)->required(), "model to display")
      ("data,d", po::value<std::filesystem::path>(&dataDir)->default_value("data"), "data directory")
      ("envmap", po::value<std::filesystem::path>(&envMap), "Environment map");

    if(app.run(argc, argv))
    {
      engine::Graphics graphics(app.clock(), app.resX(), app.resY(), dataDir);

      graphics.registerArchetype("model", engine::GraphicsArchetype{model});
      graphics.createEntity(engine::EntityId(1), "model");

      if(!envMap.empty())
      {
        graphics.addEnvmap(envMap);
      }

      glm::vec3 front(0.0f, 0.0f, -1.0f);
      glm::vec3 centre(glm::vec3(0.0f, 0.0f, 0.0f));
      glm::vec3 side(glm::vec3(1.0f, 0.0f, 0.0f));
      glm::vec3 up(glm::vec3(0.0f, 1.0f, 0.0f));
      float distance = 5;
      float yawAngle = 0;
      float pitchAngle = 0;

      bool running = true;
      float t1 = app.clock()->getTimestamp();
      while(running)
      {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
          if(event.type == SDL_QUIT)
          {
            running = false;
          }
          else if(event.type == SDL_MOUSEMOTION)
          {
            yawAngle += event.motion.xrel;
            pitchAngle = std::clamp(pitchAngle + event.motion.yrel, -85.0f, 85.0f);
          }
          else if(event.type == SDL_MOUSEWHEEL)
          {
            distance = std::clamp(distance + event.wheel.y, 0.5f, 1000.0f);
          }
        }

        float t2 = app.clock()->getTimestamp();
        float d = t2 - t1;
        BOOST_LOG_TRIVIAL(debug) << 1 / d;
        t1 = t2;

        glm::vec3 pos = front * distance;
        pos = glm::rotate(pos,
                          glm::radians(pitchAngle),
                          side);
        pos = glm::rotate(pos,
                          glm::radians(yawAngle),
                          up);

        graphics.setView(glm::lookAt(pos,
                                     centre,
                                     up));
        graphics.display();
        SDL_GL_SwapWindow(app.window());
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
