add_executable(
  viewer
  viewer.cpp)

target_include_directories(
  viewer
  PRIVATE
  ..)

target_link_libraries(
  viewer
  PRIVATE
  engine
  Boost::program_options
  Boost::log)
