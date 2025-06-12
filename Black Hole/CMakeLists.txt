cmake_minimum_required(VERSION 3.15)
  project(BlackHoleSim)

  set(CMAKE_CXX_STANDARD 17)

  # Include headers
  include_directories(${CMAKE_SOURCE_DIR})

  # Explicitly set SFML_DIR for SFML 2.x
  set(SFML_DIR "/opt/homebrew/opt/sfml@2/lib/cmake/SFML")

  # Find SFML 2.6.2
  find_package(SFML 2.6.2 REQUIRED COMPONENTS graphics window system)

  # Set the directory where SFML libraries are located (redundant but harmless with explicit paths)
  link_directories("/opt/homebrew/opt/sfml@2/lib")

  # Print SFML include directories and libraries for debugging
  message(STATUS "SFML include dirs: ${SFML_INCLUDE_DIRS}")
  message(STATUS "SFML libraries: ${SFML_LIBRARIES}")

  # Add executable
  add_executable(BlackHoleSim main.cpp BlackHole.cpp)

  # Link SFML with explicit full paths to the libraries
  target_link_libraries(BlackHoleSim 
      "/opt/homebrew/opt/sfml@2/lib/libsfml-graphics.dylib"
      "/opt/homebrew/opt/sfml@2/lib/libsfml-window.dylib"
      "/opt/homebrew/opt/sfml@2/lib/libsfml-system.dylib"
  )
  target_include_directories(BlackHoleSim PRIVATE "/opt/homebrew/opt/sfml@2/include")