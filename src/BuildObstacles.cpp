#include "BuildObstacles.h"
#include "Constants.h"
#include <cmath>


// Global variables to track current position
float currentX = INITIAL_START_X;
float currentY = INITIAL_START_Y;
float finishLineX = 0; 


//Line
Obstacle line(float horizontalOffset, float verticalOffset, float lengthAdjustment) {
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

// Ramp
Obstacle ramp(float horizontalOffset, float verticalOffset, float lengthAdjustment, float heightAdjustment) {
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


// Triangle
Obstacle triangle(float horizontalOffset, float verticalOffset, float lengthAdjustment) {
    currentX += horizontalOffset; 
    float side = TRIANGLE_SIDE + lengthAdjustment; // Side length of the equilateral triangle
    float height = (std::sqrt(3.0f) / 2.0f) * side; // Height of the equilateral triangle
    float startY = currentY + verticalOffset;

    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(currentX, startY));               // Bottom-left corner
    vertices.push_back(b2Vec2(currentX + side, startY));        // Bottom-right corner
    vertices.push_back(b2Vec2(currentX + (side / 2), startY + height)); // Top vertex

    currentX += side; // Update currentX to the end of the triangle base
    currentY = startY; // Maintain the currentY for continuity

    return {vertices, startY, startY + height, false};  
}



// Gap
Obstacle gap(float horizontalOffset, float lengthAdjustment) {
    currentX += horizontalOffset;
    float length = GAP_LENGTH + lengthAdjustment; 

    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(currentX, currentY));
    vertices.push_back(b2Vec2(currentX + length, currentY));
    currentX += length; 

    return {vertices, currentY, currentY, true}; 
}


//finish Line
Obstacle finishLine(float horizontalOffset, float verticalOffset, float lengthAdjustment) {
    currentX += horizontalOffset;
    float length = LINE_LENGTH + lengthAdjustment;
    float startY = currentY + verticalOffset;

    finishLineX = currentX; // Set finish line position

    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(currentX, startY));
    vertices.push_back(b2Vec2(currentX + length, startY));
    currentX += length;
    currentY = startY;

    return {vertices, startY, startY, false};
}




// Getter function for the finish line X position
float getFinishLineX() {
    return finishLineX;
}