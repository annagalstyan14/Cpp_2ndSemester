cmake_minimum_required(VERSION 3.15)
project(BlackHoleSim)

set(CMAKE_CXX_STANDARD 17)

# Include your headers
include_directories(${CMAKE_SOURCE_DIR})

# Add your source files
add_executable(BlackHoleSim main.cpp BlackHole.cpp)

# Link SFML
find_package(SFML 3 REQUIRED COMPONENTS graphics window system)
target_link_libraries(BlackHoleSim sfml-graphics sfml-window sfml-system)
