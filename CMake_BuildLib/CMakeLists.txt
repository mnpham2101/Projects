cmake_minimum_required(VERSION 3.15)

# set the project name and version
project(Tutorial VERSION 1.0)

configure_file(version.h  version.h)

add_executable(Tutorial main.cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)



target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

