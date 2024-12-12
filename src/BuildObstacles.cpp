#include "BuildObstacles.h"
#include "Constants.h"
#include <cmath>
#include <algorithm> 


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
Obstacle triangle(float horizontalOffset, float verticalOffset, float lengthAdjustment, float rotationDegrees) {
    currentX += horizontalOffset; 
    float side = TRIANGLE_SIDE + lengthAdjustment; // Side length of the equilateral triangle
    float height = (std::sqrt(3.0f) / 2.0f) * side; // Height of the equilateral triangle
    float startY = currentY + verticalOffset;

    // Calculate triangle vertices relative to origin
    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(0.0f, 0.0f));             // Bottom-left corner
    vertices.push_back(b2Vec2(side, 0.0f));             // Bottom-right corner
    vertices.push_back(b2Vec2(side / 2, height));       // Top vertex

    // Convert rotation from degrees to radians
    float rotationRadians = rotationDegrees * (b2_pi / 180.0f);

    // Apply rotation to each vertex
    for (auto& vertex : vertices) {
        float x = vertex.x;
        float y = vertex.y;

        vertex.x = x * std::cos(rotationRadians) - y * std::sin(rotationRadians);
        vertex.y = x * std::sin(rotationRadians) + y * std::cos(rotationRadians);

        // Translate to world position (currentX, startY)
        vertex.x += currentX;
        vertex.y += startY;
    }

    // Close the triangle by adding the first vertex again (optional)
    vertices.push_back(vertices[0]);

    // Update `currentX` to the farthest x-coordinate
    currentX = std::max_element(vertices.begin(), vertices.end(), [](const b2Vec2& a, const b2Vec2& b) {
        return a.x < b.x;
    })->x;

    // Maintain `currentY` for continuity
    currentY = startY;

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

    finishLineX = currentX; 

    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(currentX, startY));
    vertices.push_back(b2Vec2(currentX + length, startY));
    currentX += length;
    currentY = startY;

    return {vertices, startY, startY, false};
}



// Curve 
Obstacle curve(float horizontalOffset, float verticalOffset, float lengthAdjustment, float heightAdjustment, bool inward) {
    currentX += horizontalOffset;
    float startY = currentY + verticalOffset;
    float length = RAMP_LENGTH + lengthAdjustment; 
    float height = RAMP_HEIGHT + std::abs(heightAdjustment); 

    const int NUM_SEGMENTS = 20;
    std::vector<b2Vec2> vertices;
    vertices.reserve(NUM_SEGMENTS + 1);

    for (int i = 0; i <= NUM_SEGMENTS; ++i) {
        float t = static_cast<float>(i) / NUM_SEGMENTS; 
        float x = currentX + t * length;

        // Decide the y-value based on the `inward` parameter
        float y = startY + (inward ? (heightAdjustment > 0 ? -height * (1 - t) * (1 - t) : height * (1 - t) * (1 - t)) : (heightAdjustment > 0 ? height * t * t : -height * t * t));           
        vertices.push_back(b2Vec2(x, y));
    }

    currentX += length; 
    currentY = startY + (heightAdjustment > 0 ? (inward ? -height : height) : (inward ? height : -height));

    return { vertices, startY, startY + (heightAdjustment > 0 ? (inward ? -height : height) : (inward ? height : -height)), false };
}






// Getter function for the finish line X position
float getFinishLineX() {
    return finishLineX;
}