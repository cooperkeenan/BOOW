#include "BuildObstacles.h"

// Global variables to track current position
float currentX = INITIAL_START_X;
float currentY = INITIAL_START_Y;



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


//Ramp
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


//Gap
Obstacle gap(float horizontalOffset, float lengthAdjustment) {
    currentX += horizontalOffset;
    float length = GAP_LENGTH + lengthAdjustment; 

    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(currentX, currentY));
    vertices.push_back(b2Vec2(currentX + length, currentY));
    currentX += length; 

    return {vertices, currentY, currentY, true}; 
}
