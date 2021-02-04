#include "engine/core/app.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/console.hpp>

#include "engine/core/logging.h"
#include "engine/core/rtclock.h"

engine::App::App():
  _desc("Options"),
  _graphics(false),
  _resX(0),
  _resY(0),
  _clock(std::make_shared<RtClock>()),
  _window(nullptr, SDL_DestroyWindow),
  _glContext(nullptr, SDL_GL_DeleteContext)
{
  _desc.add_options()
    ("help", "Displays help")
    ("verbose,v", "Verbose logging");
}

engine::App & engine::App::enableGraphics()
{
  _graphics = true;

  _desc.add_options()
    ("x", po::value<int>(&_resX)->default_value(1280), "X resolution")
    ("y", po::value<int>(&_resY)->default_value(1024), "Y resolution")
    ("full-screen", "Full screen")
    ("vsync", "Enable vertical synchronization");

  return *this;
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

    // Logging
    BOOST_LOG_TRIVIAL(info) << "Init logging";
    engine::setLogLevel(_vm.count("verbose"));

    // Graphics
    if(_graphics)
    {
      initGraphics();
    }

    return true;
  }
}

bool engine::App::has(const std::string & option)
{
  return _vm.count(option);
}

engine::Clock * engine::App::clock()
{
  return _clock.get();
}

SDL_Window * engine::App::window()
{
  return _window.get();
}

int engine::App::resX() const
{
  return _resX;
}

int engine::App::resY() const
{
  return _resY;
}

void engine::App::initGraphics()
{
  BOOST_LOG_TRIVIAL(info) << "Init graphics";

  if(SDL_Init(SDL_INIT_VIDEO) < 0)
    throw std::runtime_error(std::string("Failed to init SDL : ") + SDL_GetError());

  unsigned int videoFlags = SDL_WINDOW_OPENGL;
  if(has("full-screen"))
  {
    SDL_DisplayMode displayMode;
    if(SDL_GetDesktopDisplayMode(0, &displayMode) < 0)
      throw std::runtime_error(std::string("Failed to get display mode : ") + SDL_GetError());
    _resX = displayMode.w;
    _resY = displayMode.h;
    videoFlags |= SDL_WINDOW_FULLSCREEN;
  }

  BOOST_LOG_TRIVIAL(info) << "Setting up video mode to " << _resX << "x" << _resY;

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  _window.reset(SDL_CreateWindow("Viewer",
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 _resX,
                                 _resY,
                                 videoFlags));

  _glContext.reset(SDL_GL_CreateContext(_window.get()));

  if(has("vsync"))
    SDL_GL_SetSwapInterval(1);
}
