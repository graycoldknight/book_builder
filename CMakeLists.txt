# Require CMAKE 3.1 or higher
cmake_minimum_required(VERSION 3.1)

# Project name
project(book_builder)

# Build for C++20 if we can
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Any source files in this directory
set(SOURCE_FILES src/main/main.cpp)

# Where any include files are
include_directories(src/include)

#add_subdirectory(src/tests)

# Name of executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})


