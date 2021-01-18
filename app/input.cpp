#include <iostream>

#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>
#include <SDL2/SDL.h>

#include "engine/core/logging.h"


namespace po = boost::program_options;

int main(int argc, char ** argv)
{
  try
  {
    po::options_description desc("Options");
    desc.add_options()
      ("help", "Displays help")
      ("verbose,v", "Verbose logging");
    
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
