#!/bin/bash

# Define directory variables
PROJECT_ROOT=$(pwd)
BUILD_DIR="${PROJECT_ROOT}/build"
EXECUTABLE_NAME="BoatOutOfWater"
VCPKG_TOOLCHAIN_FILE="/Users/cooperkeenan/Documents/GitHub/BOOW/vcpkg/scripts/buildsystems/vcpkg.cmake"

# Check if the build directory exists, if not, create it
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir "$BUILD_DIR"
fi

# Change to the build directory
cd "$BUILD_DIR"

# Run CMake to configure and generate build files using the vcpkg toolchain
echo "Running CMake with vcpkg toolchain..."
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="$VCPKG_TOOLCHAIN_FILE" ..

# Build the project
echo "Building the project..."
cmake --build .

# Check if the executable is present
if [ -f "$EXECUTABLE_NAME" ]; then
    echo "Running the executable..."
    ./"$EXECUTABLE_NAME"
else
    echo "Error: Executable not found. Build might have failed."
fi

# Change back to the project root
cd "$PROJECT_ROOT"
