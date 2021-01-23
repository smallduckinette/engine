#ifndef __CORE_APP_H__
#define __CORE_APP_H__

#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace engine
{
  class App
  {
  public:
    App();

    /// Add additional options to the application
    po::options_description_easy_init add_options();

    /// Parse program parameters
    bool run(int argc, char ** argv);

    /// Return true if the option was set
    bool has(const std::string & option);

  private:
    po::options_description _desc;
    po::variables_map _vm;
  };
}

#endif
