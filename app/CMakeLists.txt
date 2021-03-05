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
  engine)

add_executable(
  dj
  dj.cpp)

target_link_libraries(
  dj
  PRIVATE
  engine)
