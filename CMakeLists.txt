cmake_minimum_required(VERSION 3.15)
project(Milton VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Load dependencies with CMakeDeps generator
find_package(ftxui REQUIRED)
find_package(yaml-cpp REQUIRED)

# Add subdirectory for source files
add_subdirectory(src)

# Specify where to find header files
include_directories(${YAMLCPP_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

target_link_libraries(Milton ftxui::ftxui yaml-cpp)

set(CMAKE_CXX_FLAGS "-static-libgcc -static-libstdc++")
