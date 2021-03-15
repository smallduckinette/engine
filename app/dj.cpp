#include <boost/log/trivial.hpp>

#include "engine/core/app.h"
#include "engine/sounds/streamable.h"
#include "engine/sounds/music.h"

#include <iostream>

namespace po = boost::program_options;

int main(int argc, char ** argv)
{
  try
  {
    std::filesystem::path audioFile;

    engine::App app;
    app.add_options()
      ("file,f", po::value<std::filesystem::path>(&audioFile), "Ogg/Vorbis audio file");

    app.enableAudio();

    if(app.run(argc, argv))
    {
      auto streamable = std::make_shared<engine::VorbisStreamable>(audioFile);
      engine::Music music;
      music.play(streamable);

      std::string command;
      do
      {
        std::getline(std::cin, command);
      }
      while(command != "q");
    }

    return 0;
  }
  catch(const std::exception & e)
  {
    BOOST_LOG_TRIVIAL(error) << e.what();
    return -1;
  }
}
