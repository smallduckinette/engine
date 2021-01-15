add_executable(
  engine_test
  test_gltf.cpp
  test_interpolator.cpp
  test_main.cpp)

target_include_directories(
  engine_test
  PRIVATE
  ..)

target_link_libraries(
  engine_test
  Boost::unit_test_framework
  engine)

add_test(
  NAME Engine
  COMMAND engine_test)
