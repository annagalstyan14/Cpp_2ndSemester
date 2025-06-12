cmake_minimum_required(VERSION 3.15)
  project(BlackHoleSim)

  set(CMAKE_CXX_STANDARD 17)

  # Explicitly set include directories
  include_directories(${CMAKE_SOURCE_DIR})
  include_directories(/opt/homebrew/opt/sfml@2/include)

  # Find SFML with custom path
  set(SFML_DIR "/opt/homebrew/opt/sfml@2/lib/cmake/SFML")
  find_package(SFML 2.6.2 REQUIRED COMPONENTS graphics window system)

  # Print SFML include directories and libraries for debugging
  message(STATUS "SFML include dirs: ${SFML_INCLUDE_DIRS}")
  message(STATUS "SFML libraries: ${SFML_LIBRARIES}")

  # Add executable
  add_executable(BlackHoleSim main.cpp BlackHole.cpp Space.cpp)

  # Link SFML libraries explicitly
  target_link_libraries(BlackHoleSim sfml-graphics sfml-window sfml-system)
  target_include_directories(BlackHoleSim PRIVATE ${SFML_INCLUDE_DIRS})