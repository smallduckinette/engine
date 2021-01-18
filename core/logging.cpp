#include "engine/core/logging.h"


#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/console.hpp>

void engine::setLogLevel(bool verbose)
{
  if(verbose)
  {
    boost::log::core::get()->set_filter
      (boost::log::trivial::severity >= boost::log::trivial::debug);
  }
  else
  {
    boost::log::core::get()->set_filter
      (boost::log::trivial::severity >= boost::log::trivial::info);
  }  
}
