cmake_minimum_required(VERSION 3.15)
  project(BlackHoleSim)

  set(CMAKE_CXX_STANDARD 17)

  # Include headers
  include_directories(${CMAKE_SOURCE_DIR})

  # Find SFML with minimal components for testing
  find_package(SFML 3.0.1 REQUIRED COMPONENTS system)

  # Print detailed SFML configuration for debugging
  message(STATUS "SFML include dirs: ${SFML_INCLUDE_DIRS}")
  message(STATUS "SFML libraries: ${SFML_LIBRARIES}")
  message(STATUS "SFML found components: ${SFML_LIBRARIES}")

  # Add executable
  add_executable(BlackHoleSim main.cpp BlackHole.cpp)

  # Link SFML
  target_link_libraries(BlackHoleSim ${SFML_LIBRARIES})
  target_include_directories(BlackHoleSim PRIVATE ${SFML_INCLUDE_DIRS})