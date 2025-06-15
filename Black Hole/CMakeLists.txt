cmake_minimum_required(VERSION 3.15)
project(BlackHoleSim)

set(CMAKE_CXX_STANDARD 17)

# Explicitly set SFML directory to the downloaded SFML 2.6.2
set(SFML_ROOT "/Users/annagalstyan/Downloads/SFML-2.6.2 2")
set(SFML_DIR "${SFML_ROOT}/lib/cmake/SFML")

# Include directories
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${SFML_ROOT}/include)

# Find SFML 2.6
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Add executable with all source files
add_executable(BlackHoleSim
    main.cpp
    BlackHole.cpp
    Space.cpp
)

# Set RPATH for macOS to find SFML dynamic libraries at runtime
set_target_properties(BlackHoleSim PROPERTIES
    BUILD_RPATH "${SFML_ROOT}/lib;@loader_path;@loader_path/Frameworks"
    INSTALL_RPATH "${SFML_ROOT}/lib;@loader_path;@loader_path/Frameworks"
)

# Link SFML libraries
target_link_libraries(BlackHoleSim PRIVATE sfml-graphics sfml-window sfml-system)

# Copy SFML dynamic libraries to the build directory
file(GLOB SFML_DYLIBS "${SFML_ROOT}/lib/*.dylib")
file(COPY ${SFML_DYLIBS} DESTINATION ${CMAKE_BINARY_DIR})

# Copy freetype framework to the build directory
file(COPY "${SFML_ROOT}/extlibs/freetype.framework" DESTINATION ${CMAKE_BINARY_DIR}/Frameworks)