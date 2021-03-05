#include <boost/log/trivial.hpp>

#include "engine/core/app.h"

namespace po = boost::program_options;

int main(int argc, char ** argv)
{
  try
  {
    engine::App app;

    app.enableAudio();

    if(app.run(argc, argv))
    {
    }

    return 0;
  }
  catch(const std::exception & e)
  {
    BOOST_LOG_TRIVIAL(error) << e.what();
    return -1;
  }
}
