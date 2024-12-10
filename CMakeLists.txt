# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and a few useful settings. Other commands can pick up the results
project(BoatOutOfWater)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Add source files
file(GLOB SOURCES "src/*.cpp" "src/levels/*.cpp")

# Add the executable
add_executable(BoatOutOfWater ${SOURCES})

# Ensure vcpkg is used
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "CMAKE_TOOLCHAIN_FILE must be set to the vcpkg toolchain file.")
endif()

# Find and link SFML
find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system audio)

# Find and link Box2D
find_package(box2d CONFIG REQUIRED)

# Link libraries
target_link_libraries(BoatOutOfWater PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    box2d::box2d
)

# Output the build type for debugging
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
