#include "engine/core/app.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <SDL2/SDL_ttf.h>

#include "engine/core/logging.h"
#include "engine/core/rtclock.h"
#include "engine/core/audio.h"

engine::App::App():
  _desc("Options"),
  _graphics(false),
  _audio(false),
  _asio(false),
  _resX(0),
  _resY(0),
  _nbAsioThreads(0),
  _clock(std::make_shared<RtClock>()),
  _window(nullptr, SDL_DestroyWindow),
  _glContext(nullptr, SDL_GL_DeleteContext),
  _alcDevice(nullptr, alcCloseDevice),
  _alcContext(nullptr, alcDestroyContext)
{
  _desc.add_options()
    ("help", "Displays help")
    ("verbose,v", "Verbose logging");
}

engine::App::~App()
{
  if(TTF_WasInit())
  {
    TTF_Quit();
  }

  stopAsio();
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

engine::App & engine::App::enableAudio()
{
  _audio = true;

  return *this;
}

engine::App & engine::App::enableAsio()
{
  _asio = true;

  _desc.add_options()
    ("threads", po::value<unsigned int>(&_nbAsioThreads)->default_value(4), "Number of working threads");

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

    // Asio
    if(_asio)
    {
      initAsio();
    }

    // Graphics
    if(_graphics)
    {
      initGraphics();
    }

    // Fonts
    initTTF();

    // Audio
    if(_audio)
    {
      initAudio();
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

ALCdevice * engine::App::audioDevice()
{
  return _alcDevice.get();
}

ALCcontext * engine::App::audioContext()
{
  return _alcContext.get();
}

boost::asio::io_context * engine::App::asioContext()
{
  return _asioContext.get();
}

void engine::App::stopAsio()
{
  if(_asio)
  {
    asioContext()->stop();
    for(auto && thread : _asioThreads)
    {
      thread.join();
    }
    _asioThreads.clear();
  }
}

void engine::App::initAsio()
{
  BOOST_LOG_TRIVIAL(info) << "Init threads";

  _asioContext = std::make_shared<boost::asio::io_context>();
  _workGuard = std::make_shared<work_guard>(_asioContext->get_executor());

  for (unsigned int index = 0; index < _nbAsioThreads; ++index)
  {
    _asioThreads.push_back(std::thread([this](){ _asioContext->run(); }));
    pthread_setname_np(_asioThreads.back().native_handle(), "asio");
  }

  BOOST_LOG_TRIVIAL(info) << "Created " << _nbAsioThreads << " work threads";
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

  SDL_GL_SetSwapInterval(has("vsync") ? 1 : 0);
}

void engine::App::initTTF()
{
  if(TTF_Init() != 0)
  {
    throw std::runtime_error(std::string("Failed to initialise TTF : ") + TTF_GetError());
  }
}

void engine::App::initAudio()
{
  BOOST_LOG_TRIVIAL(info) << "Init audio";

  const char * deviceName = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
  if(alcGetError(nullptr) != ALC_NO_ERROR)
    throw std::runtime_error(std::string("Failed to get default audio device : ")
                             + getAlcErrorString());

  BOOST_LOG_TRIVIAL(info) << "Default audio device is " << deviceName;

  _alcDevice.reset(alcOpenDevice(deviceName));
  if(alcGetError(nullptr) != ALC_NO_ERROR)
    throw std::runtime_error(std::string("Failed to initialise audio : ")
                             + getAlcErrorString());

  _alcContext.reset(alcCreateContext(_alcDevice.get(), nullptr));
  if(alcGetError(nullptr) != ALC_NO_ERROR)
    throw std::runtime_error(std::string("Failed to initialise audio : ")
                             + getAlcErrorString());

  if(!alcMakeContextCurrent(audioContext()))
    throw std::runtime_error(std::string("Failed to make audio context current : ")
                             + getAlcErrorString());
}
