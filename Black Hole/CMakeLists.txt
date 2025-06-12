cmake_minimum_required(VERSION 3.10)
project(BlackHoleSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

# Add source files
set(SOURCES
    main.cpp
    BlackHole.cpp
)

# Create executable
add_executable(BlackHoleSim ${SOURCES})

# Link SFML libraries
if (SFML_FOUND)
    target_link_libraries(BlackHoleSim PRIVATE SFML::Graphics SFML::Window SFML::System)
endif()

# No longer need to copy shader files
# file(COPY ${CMAKE_SOURCE_DIR}/space.vert DESTINATION ${CMAKE_BINARY_DIR})
# file(COPY ${CMAKE_SOURCE_DIR}/space.frag DESTINATION ${CMAKE_BINARY_DIR})
