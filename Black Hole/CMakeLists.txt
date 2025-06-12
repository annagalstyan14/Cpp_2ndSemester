cmake_minimum_required(VERSION 3.15)
  project(BlackHoleSim)

  set(CMAKE_CXX_STANDARD 17)

  # Include headers
  include_directories(${CMAKE_SOURCE_DIR})

  # Find SFML
  find_package(SFML 3.0.1 REQUIRED COMPONENTS graphics window system)

  # Print SFML include directories for debugging
  message(STATUS "SFML include dirs: ${SFML_INCLUDE_DIRS}")

  # Add executable
  add_executable(BlackHoleSim main.cpp BlackHole.cpp)

  # Link SFML and include directories
  target_link_libraries(BlackHoleSim SFML::Graphics SFML::Window SFML::System)
  target_include_directories(BlackHoleSim PRIVATE ${SFML_INCLUDE_DIRS})