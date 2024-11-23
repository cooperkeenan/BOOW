# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(BoatOutOfWater)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Add source files
file(GLOB SOURCES "src/*.cpp")

# Add the executable
add_executable(BoatOutOfWater ${SOURCES})

# Find and link SFML
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)
target_link_libraries(BoatOutOfWater sfml-graphics sfml-window sfml-system)

# Find and link Box2D
find_library(BOX2D_LIBRARY Box2D PATHS /opt/homebrew/lib /opt/homebrew/Cellar/box2d/2.4.2/lib)
find_path(BOX2D_INCLUDE_DIR Box2D PATHS /opt/homebrew/include /opt/homebrew/Cellar/box2d/2.4.2/include)
include_directories(${BOX2D_INCLUDE_DIR})
target_link_libraries(BoatOutOfWater ${BOX2D_LIBRARY})
