#include <boost/log/trivial.hpp>

#include "engine/core/app.h"
#include "engine/sounds/streamable.h"

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
      engine::VorbisStreamable streamable(audioFile);

      char data[4096];
      size_t readData = 0;
      size_t totalData = 0;
      while((readData = streamable.stream(data, 4096)) > 0)
      {
        totalData += readData;
        std::cout << "We have read " << readData << ", total " << totalData << std::endl;
      }

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
