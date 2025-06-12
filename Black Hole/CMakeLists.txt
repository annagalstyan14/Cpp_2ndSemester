cmake_minimum_required(VERSION 3.10)
project(BlackHoleSim)

set(CMAKE_CXX_STANDARD 17)

add_executable(BlackHoleSim
    main.cpp
    BlackHole.cpp
    Space.cpp
    gl.c
    Shader.cpp
    stb_image_impl.cpp
)

# Include directories
target_include_directories(BlackHoleSim PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# Link libraries
target_link_libraries(BlackHoleSim
    glfw
    ${OPENGL_gl_LIBRARY}
)
