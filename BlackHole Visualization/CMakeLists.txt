cmake_minimum_required(VERSION 3.10)
project(BlackHoleSimulation LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable compile_commands.json for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SFML 2.x
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add executable
add_executable(blackhole
    main.cpp
    Vector2D.hpp
    Photon.hpp
    BlackHole.hpp
)

# Include directories
target_include_directories(blackhole PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link SFML libraries
target_link_libraries(blackhole PRIVATE sfml-graphics sfml-window sfml-system)

# Set output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)