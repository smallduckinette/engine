add_library(
  engine
  SHARED
  adh/animation.cpp
  adh/camera.cpp
  adh/context.cpp
  adh/envmap.cpp
  adh/hud.cpp
  adh/node.cpp
  adh/primitive.cpp
  adh/shader.cpp
  adh/texture.cpp
  adh/texturecubemap.cpp
  adh/transform.cpp
  controls/bindings.cpp
  controls/controls.cpp
  core/app.cpp
  core/audio.cpp
  core/logging.cpp
  core/json_utils.cpp
  core/rtclock.cpp
  core/serializejson.cpp
  entity/entitylibrary.cpp
  gltf/asset.cpp
  gltf/builder.cpp
  graphics/graphics.cpp
  sounds/music.cpp
  sounds/sounds.cpp
  sounds/streamable.cpp)

target_include_directories(
  engine
  PUBLIC
  ..)

target_link_libraries(
  engine
  PUBLIC
  Boost::log
  Boost::program_options
  GLEW::GLEW
  OpenGL::OpenGL
  PkgConfig::JSONCPP
  PkgConfig::OPENAL
  PkgConfig::SDL2
  PkgConfig::SDL2IMAGE
  PkgConfig::SDL2TTF
  PkgConfig::VORBIS
  PkgConfig::VORBISFILE)

add_subdirectory(app)
add_subdirectory(tests)
