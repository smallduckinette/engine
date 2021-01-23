#include "engine/core/app.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/console.hpp>

#include "engine/core/logging.h"

engine::App::App():
  _desc("Options")
{
  _desc.add_options()
    ("help", "Displays help")
    ("verbose,v", "Verbose logging");
}

po::options_description_easy_init engine::App::add_options()
{
  return _desc.add_options();
}

bool engine::App::run(int argc, char ** argv)
{
  po::store(po::parse_command_line(argc, argv, _desc), _vm);

  if (_vm.count("help"))
  {
    std::cout << _desc << std::endl;
    return false;
  }
  else
  {
    po::notify(_vm);
    engine::setLogLevel(_vm.count("verbose"));
    return true;
  }
}

bool engine::App::has(const std::string & option)
{
  return _vm.count(option);
}
