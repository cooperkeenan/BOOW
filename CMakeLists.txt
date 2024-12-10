# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(BoatOutOfWater)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Add source files
# Gather all .cpp files in src and src/levels
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

# Set _ITERATOR_DEBUG_LEVEL dynamically based on build type
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MDd /D_ITERATOR_DEBUG_LEVEL=2")
    target_compile_definitions(BoatOutOfWater PRIVATE _DEBUG)
    target_compile_options(BoatOutOfWater PRIVATE /MDd)
    target_link_libraries(BoatOutOfWater PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        box2d::box2d
    )
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MD /D_ITERATOR_DEBUG_LEVEL=0")
    target_compile_definitions(BoatOutOfWater PRIVATE NDEBUG)
    target_compile_options(BoatOutOfWater PRIVATE /MD)
    target_link_libraries(BoatOutOfWater PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        box2d::box2d
    )
endif()

# Output the build type for debugging
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
