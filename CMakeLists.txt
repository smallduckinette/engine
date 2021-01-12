add_library(
  engine
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
  entity/entityfactory.cpp)

target_include_directories(
  engine
  PRIVATE
  .)

target_link_libraries(
  engine
  PRIVATE
  PkgConfig::JSONCPP)

find_package(Boost 1.71 COMPONENTS system unit_test_framework)

add_executable(
  engine_test
  tests/test_interpolator.cpp
  tests/test_main.cpp)

target_include_directories(
  engine_test
  PRIVATE
  .)

target_link_libraries(
  engine_test
  Boost::unit_test_framework
  engine)

add_test(
  NAME Engine
  COMMAND engine_test)
