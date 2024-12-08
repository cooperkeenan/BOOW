#include "BuildObstacles.h"

// Constants for standard dimensions
constexpr float INITIAL_START_X = -20.0f;
constexpr float INITIAL_START_Y = 20.0f;
constexpr float LINE_LENGTH = 10.0f;
constexpr float RAMP_LENGTH = 10.0f;
constexpr float RAMP_HEIGHT = 5.0f;
constexpr float GAP_LENGTH = 5.0f;

// Global variables to track current position
float currentX = INITIAL_START_X;
float currentY = INITIAL_START_Y;

//Starting Line
Obstacle startingLine() {
    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(currentX, currentY));
    vertices.push_back(b2Vec2(currentX + LINE_LENGTH, currentY));
    currentX += LINE_LENGTH;  
    return {vertices, currentY, currentY, false};  // isGap = false
}


// Create Line
Obstacle line(float horizontalOffset = 0.0f, float verticalOffset = 0.0f, float lengthAdjustment = 0.0f) {
    currentX += horizontalOffset; 
    float length = LINE_LENGTH + lengthAdjustment; 
    float startY = currentY + verticalOffset;
    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(currentX, startY));
    vertices.push_back(b2Vec2(currentX + length, startY));
    currentX += length; 
    currentY = startY;  
    return {vertices, startY, startY, false};  
}

// Create Ramp
Obstacle ramp(float horizontalOffset = 0.0f, float verticalOffset = 0.0f, float lengthAdjustment = 0.0f, float heightAdjustment = 0.0f) {
    currentX += horizontalOffset; 
    float length = RAMP_LENGTH + lengthAdjustment; 
    float height = RAMP_HEIGHT + heightAdjustment; 
    float startY = currentY + verticalOffset;
    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(currentX, startY));
    vertices.push_back(b2Vec2(currentX + length, startY + height));
    currentX += length;  
    currentY = startY + height;  
    return {vertices, startY, currentY, false};
}

// Create Gap (Invisible Line)
Obstacle gap(float horizontalOffset = 0.0f, float length = GAP_LENGTH) {
    currentX += horizontalOffset;
    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(currentX, currentY));
    vertices.push_back(b2Vec2(currentX + length, currentY));
    currentX += length; 
    return {vertices, currentY, currentY, true};  // isGap = true
}

// Function to create a complete obstacle set
std::vector<Obstacle> Obstacle_1() {
    std::vector<Obstacle> obstacleSet;

    obstacleSet.push_back(startingLine());
    obstacleSet.push_back(ramp());
    obstacleSet.push_back(gap(50));
    obstacleSet.push_back(line(0, -5));  

    return obstacleSet;
}

