cmake_minimum_required(VERSION 3.10)
project(BlackHoleSim)

set(CMAKE_CXX_STANDARD 17)

add_executable(BlackHoleSim
    main.cpp
    BlackHole.cpp
    Space.cpp
    gl.c
)

# Add include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# Link OpenGL and GLFW
target_link_libraries(BlackHoleSim
    glfw
    ${OPENGL_gl_LIBRARY}
)
