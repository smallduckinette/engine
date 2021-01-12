add_library(
  engine
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
