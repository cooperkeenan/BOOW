# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(BoatOutOfWater)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Add source files
file(GLOB SOURCES "src/*.cpp" "src/levels/*.cpp")

# Add the executable
add_executable(BoatOutOfWater ${SOURCES})

# Find and link SFML
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)
target_link_libraries(BoatOutOfWater PRIVATE sfml-graphics sfml-window sfml-system)

# Find and link Box2D
find_library(BOX2D_LIBRARY Box2D PATHS /opt/homebrew/lib /opt/homebrew/Cellar/box2d/2.4.2/lib)
find_path(BOX2D_INCLUDE_DIR Box2D PATHS /opt/homebrew/include /opt/homebrew/Cellar/box2d/2.4.2/include)
include_directories(${BOX2D_INCLUDE_DIR})

# Note: Adjust this to the actual path or link by the found library name
# If the path you provided is consistent across platforms, it's okay to use a direct path
# Otherwise, you should rely on find_library() to provide the proper link directive
if(BOX2D_LIBRARY)
    target_link_libraries(BoatOutOfWater PRIVATE ${BOX2D_LIBRARY})
else()
    message(FATAL_ERROR "Box2D library not found")
endif()