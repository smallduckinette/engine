#include <iostream>
#include <filesystem>
#include <fstream>

#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>
#include <SDL2/SDL.h>

#include "engine/core/logging.h"
#include "engine/core/json_utils.h"
#include "engine/controls/controls.h"


namespace po = boost::program_options;

/// Emulate a bunch of user bindings from the controls config
void makeUserBindings(const Json::Value & config,
                      std::map<std::string, engine::Controls::BindingID> & nameToId,
                      std::map<engine::Controls::BindingID, std::string> & idToName)
{
  if(config != Json::Value())
  {
    for(auto && device : engine::getNodeOrThrow(config, "devices"))
    {
      for(auto && binding : engine::getNodeOrThrow(device, "bindings"))
      {
        std::string name;
        engine::get(binding, "name", name);
        engine::Controls::BindingID bindingID(nameToId.size() + 1);
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

    po::options_description desc("Options");
    desc.add_options()
      ("help", "Displays help")
      ("verbose,v", "Verbose logging")
      ("config,c", po::value<std::filesystem::path>(&config), "Control configuration file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help"))
    {
      std::cout << desc << std::endl;
    }
    else
    {
      po::notify(vm);
      engine::setLogLevel(vm.count("verbose"));

      BOOST_LOG_TRIVIAL(info) << "Init SDL";

      Json::Value jsonConfig;
      if(!config.empty())
      {
        std::ifstream str(config);
        str >> jsonConfig;
      }

      std::map<std::string, engine::Controls::BindingID> nameToId;
      std::map<engine::Controls::BindingID, std::string> idToName;
      makeUserBindings(jsonConfig, nameToId, idToName);
      engine::Controls controls(jsonConfig, nameToId);

      controls.onButton().connect
        ([&idToName](engine::Controls::BindingID bindingID)
         {
           auto it = idToName.find(bindingID);
           if(it != idToName.end())
           {
             BOOST_LOG_TRIVIAL(info) << "onButton[" << it->second << "]";
           }
         });

      controls.onAxis().connect
        ([&idToName](engine::Controls::BindingID bindingID, int value)
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
