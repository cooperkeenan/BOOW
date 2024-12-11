# Navigate to the project root directory
cd "C:\Users\Kai\Documents\Group_Games\BOOW"

# Remove the existing build directory if it exists
if (Test-Path -Path "build") {
    Remove-Item -Path "build" -Recurse -Force
}

# Create a new build directory
New-Item -Path "build" -ItemType "directory"

# Change directory to the newly created build directory
cd build

# Run cmake to configure the project and generate the build system
cmake .. -DCMAKE_TOOLCHAIN_FILE="C:\Users\Kai\Documents\Group_Games\vcpkg\scripts\buildsystems\vcpkg.cmake" -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build . --config Release

# Change directory to the Release folder
cd Release

# Execute the built application
.\BoatOutOfWater.exe
