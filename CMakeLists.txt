add_library(
  engine
  SHARED
  adh/animation.cpp
  adh/camera.cpp
  adh/context.cpp
  adh/node.cpp
  adh/primitive.cpp
  adh/rtclock.cpp
  adh/shader.cpp
  adh/texture.cpp
  adh/transform.cpp
  core/json_utils.cpp
  entity/entityfactory.cpp
  gltf/asset.cpp
  gltf/builder.cpp)

target_include_directories(
  engine
  PUBLIC
  .)

target_link_libraries(
  engine
  PUBLIC
  PkgConfig::JSONCPP
  OpenGL::OpenGL
  GLEW::GLEW
  PkgConfig::SDL2
  PkgConfig::SDL2IMAGE)

add_subdirectory(app)
add_subdirectory(tests)
