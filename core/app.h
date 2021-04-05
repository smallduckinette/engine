#ifndef __CORE_APP_H__
#define __CORE_APP_H__

#include <boost/program_options.hpp>
#include <boost/asio.hpp>
#include <SDL2/SDL.h>
#include <AL/al.h>
#include <AL/alc.h>

namespace po = boost::program_options;

namespace engine
{
  class Clock;

  class App
  {
  public:
    App();
    ~App();

    App & enableGraphics();

    App & enableAudio();

    App & enableAsio();

    /// Add additional options to the application
    po::options_description_easy_init add_options();

    /// Parse program parameters
    bool run(int argc, char ** argv);

    /// Return true if the option was set
    bool has(const std::string & option);

    /// The application clock
    Clock * clock();

    /// The SDL window
    SDL_Window * window();

    /// X resolution
    int resX() const;

    /// Y resolution
    int resY() const;

    /// The audio device
    ALCdevice * audioDevice();

    /// The audio context
    ALCcontext * audioContext();

    /// The asio context
    boost::asio::io_context * asioContext();

    /// Turn off asio context
    void stopAsio();

  private:
    void initAsio();
    void initGraphics();
    void initTTF();
    void initAudio();

    po::options_description _desc;
    po::variables_map _vm;

    bool _graphics;
    bool _audio;
    bool _asio;

    // Graphics parameters
    int _resX;
    int _resY;

    // Asio parameters
    unsigned int _nbAsioThreads;

    // Services
    std::shared_ptr<Clock> _clock;

    std::shared_ptr<boost::asio::io_context> _asioContext;
    using work_guard = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
    std::shared_ptr<work_guard> _workGuard;
    std::vector<std::thread> _asioThreads;

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window;
    std::unique_ptr<void, decltype(&SDL_GL_DeleteContext)> _glContext;

    std::unique_ptr<ALCdevice, decltype(&alcCloseDevice)> _alcDevice;
    std::unique_ptr<ALCcontext, decltype(&alcDestroyContext)> _alcContext;
  };
}

#endif
