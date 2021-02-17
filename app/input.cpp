#include <iostream>
#include <filesystem>
#include <fstream>

#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>
#include <SDL2/SDL.h>

#include "engine/core/app.h"
#include "engine/core/json_utils.h"
#include "engine/controls/controls.h"


namespace po = boost::program_options;

/// Emulate a bunch of user bindings from the controls config
void makeUserBindings(const Json::Value & config,
                      std::map<std::string, engine::Controls::BindingId> & nameToId,
                      std::map<engine::Controls::BindingId, std::string> & idToName)
{
  if(config != Json::Value())
  {
    for(auto && device : engine::getNodeOrThrow(config, "devices"))
    {
      for(auto && binding : engine::getNodeOrThrow(device, "bindings"))
      {
        std::string name;
        engine::get(binding, "name", name);
        engine::Controls::BindingId bindingID(nameToId.size() + 1);
        nameToId.emplace(name, bindingID);
        idToName.emplace(bindingID, name);
      }
    }
  }
}

int main(int argc, char ** argv)
{
  try
  {
    std::filesystem::path config;

    engine::App app;
    app.add_options()
      ("config,c", po::value<std::filesystem::path>(&config), "Control configuration file");

    if(app.run(argc, argv))
    {
      BOOST_LOG_TRIVIAL(info) << "Init SDL";

      engine::Controls controls;

      std::map<std::string, engine::Controls::BindingId> nameToId;
      std::map<engine::Controls::BindingId, std::string> idToName;
      if(!config.empty())
      {
        Json::Value jsonConfig;
        std::ifstream str(config);
        str >> jsonConfig;
        makeUserBindings(jsonConfig, nameToId, idToName);
        controls.initConfig(jsonConfig, nameToId);
      }

      controls.onButton().connect
        ([&idToName](engine::Controls::BindingId bindingID)
         {
           auto it = idToName.find(bindingID);
           if(it != idToName.end())
           {
             BOOST_LOG_TRIVIAL(info) << "onButton[" << it->second << "]";
           }
         });

      controls.onAxis().connect
        ([&idToName](engine::Controls::BindingId bindingID, float value)
         {
           auto it = idToName.find(bindingID);
           if(it != idToName.end())
           {
             BOOST_LOG_TRIVIAL(info) << "onAxis[" << it->second << "] " << value;
           }
         });

      bool running = true;
      while(running)
      {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
          if(event.type == SDL_QUIT)
          {
            running = false;
          }
          else
          {
            controls.process(event);
          }
        }

        SDL_Delay(20);
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
