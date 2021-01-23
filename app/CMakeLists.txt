add_executable(
  viewer
  viewer.cpp)

target_link_libraries(
  viewer
  PRIVATE
  engine)

add_executable(
  input
  input.cpp)

target_link_libraries(
  input
  PRIVATE
  engine
  Boost::program_options
  Boost::log)
