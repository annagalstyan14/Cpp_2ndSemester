cmake_minimum_required(VERSION 3.10)

# Set the project name
project(NSExpression_CPP)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
set(SOURCES
    src/main.cpp
    src/Evaluator.cpp
    src/Lexer.cpp
    src/Parser.cpp
    src/Token.cpp
    src/Utilities.cpp
    src/FunctionAnalyzer.cpp
)
if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(NSExpression_CPP ${CURSES_LIBRARIES})