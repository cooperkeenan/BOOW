# Remove the existing build directory if it exists
if (Test-Path -Path "build") {
    Remove-Item -Recurse -Force "build"
}

# Create a new build directory
New-Item -ItemType Directory -Name "build" | Out-Null

# Navigate into the build directory
Set-Location build

# Configure the project with CMake
cmake .. -DCMAKE_TOOLCHAIN_FILE="C:/Users/Kai/Documents/Games_Engineering/vcpkg/scripts/buildsystems/vcpkg.cmake"

# Build the project
cmake --build .

# Move to exe location
Set-Location Debug

# Run the resulting executable (change the name if different)
./BoatOutOfWater.exe
