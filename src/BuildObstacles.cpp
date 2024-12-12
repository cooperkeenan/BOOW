#include "BuildObstacles.h"
#include "Constants.h"
#include <cmath>
#include <algorithm> 


// Global variables to track the current drawing position on the track
float currentX = INITIAL_START_X;
float currentY = INITIAL_START_Y;
float finishLineX = 0; 


// Creates a straight line obstacle with optional horizontal/vertical offsets and length adjustments.
Obstacle line(float horizontalOffset, float verticalOffset, float lengthAdjustment) {

    currentX += horizontalOffset; 
    float length = LINE_LENGTH + lengthAdjustment; 
    float startY = currentY + verticalOffset;

    // Two-point line
    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(currentX, startY));
    vertices.push_back(b2Vec2(currentX + length, startY));
    
    // Update current position for next obstacle
    currentX += length; 
    currentY = startY;  
    
    return {vertices, startY, startY, false};  
}


// Creates a ramp obstacle that slopes upwards or downwards.
Obstacle ramp(float horizontalOffset, float verticalOffset, float lengthAdjustment, float heightAdjustment) {
    currentX += horizontalOffset; 
    float length = RAMP_LENGTH + lengthAdjustment; 
    float height = RAMP_HEIGHT + heightAdjustment; 
    float startY = currentY + verticalOffset;

    // Ramp defined by two vertices: start and end at a higher/lower point
    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(currentX, startY));
    vertices.push_back(b2Vec2(currentX + length, startY + height));
    
    // Update current position
    currentX += length;  
    currentY = startY + height;  

    return {vertices, startY, currentY, false};
}


// Creates a rotated equilateral triangle obstacle.
Obstacle triangle(float horizontalOffset, float verticalOffset, float lengthAdjustment, float rotationDegrees) {
    currentX += horizontalOffset; 
    float side = TRIANGLE_SIDE + lengthAdjustment; 
    float height = (std::sqrt(3.0f) / 2.0f) * side; 
    float startY = currentY + verticalOffset;

    // Define a triangle oriented initially without rotation
    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(0.0f, 0.0f));
    vertices.push_back(b2Vec2(side, 0.0f));
    vertices.push_back(b2Vec2(side / 2, height));

    // Apply rotation to the triangle's vertices
    float rotationRadians = rotationDegrees * (b2_pi / 180.0f);
    for (auto& vertex : vertices) {
        float x = vertex.x;
        float y = vertex.y;
        vertex.x = x * std::cos(rotationRadians) - y * std::sin(rotationRadians);
        vertex.y = x * std::sin(rotationRadians) + y * std::cos(rotationRadians);

        // Translate the rotated vertices to the current track position
        vertex.x += currentX;
        vertex.y += startY;
    }

    // Close the triangle by repeating the first vertex
    vertices.push_back(vertices[0]);

    // Update currentX to the farthest x-coordinate of the triangle to continue from its end
    currentX = std::max_element(vertices.begin(), vertices.end(),[](const b2Vec2& a, const b2Vec2& b){ return a.x < b.x; })->x;

    // Keep currentY aligned at the triangle's starting height
    currentY = startY;

    return {vertices, startY, startY + height, false};  
}


// Creates a gap obstacle
Obstacle gap(float horizontalOffset, float lengthAdjustment) {
    currentX += horizontalOffset;
    float length = GAP_LENGTH + lengthAdjustment; 

    // Gap is just a line segment that is considered a "no ground" zone (isGap=true)
    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(currentX, currentY));
    vertices.push_back(b2Vec2(currentX + length, currentY));
    currentX += length; 

    return {vertices, currentY, currentY, true}; 
}


// Creates a finish line obstacle. Crossing this line may trigger level completion.
Obstacle finishLine(float horizontalOffset, float verticalOffset, float lengthAdjustment) {
    currentX += horizontalOffset;
    float length = LINE_LENGTH + lengthAdjustment;
    float startY = currentY + verticalOffset;

    // Record the finish line position for game logic elsewhere
    finishLineX = currentX; 

    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(currentX, startY));
    vertices.push_back(b2Vec2(currentX + length, startY));
    currentX += length;
    currentY = startY;

    return {vertices, startY, startY, false};
}


// Creates a curved obstacle, either convex or concave depending on `inward`.
Obstacle curve(float horizontalOffset, float verticalOffset, float lengthAdjustment, float heightAdjustment, bool inward) {
    currentX += horizontalOffset;
    float startY = currentY + verticalOffset;
    float length = RAMP_LENGTH + lengthAdjustment; 
    float height = RAMP_HEIGHT + std::abs(heightAdjustment); 

    const int NUM_SEGMENTS = 20;
    std::vector<b2Vec2> vertices;
    vertices.reserve(NUM_SEGMENTS + 1);

    // Generate curve points by interpolating from start to end
    // Shape depends on 'inward' and sign of heightAdjustment
    for (int i = 0; i <= NUM_SEGMENTS; ++i) {
        float t = static_cast<float>(i) / NUM_SEGMENTS; 
        float x = currentX + t * length;

        // Quadratic shape to define the curve profile
        float y = startY + (inward ? (heightAdjustment > 0 ? -height*(1-t)*(1-t) : height*(1-t)*(1-t))
                                   : (heightAdjustment > 0 ? height*t*t : -height*t*t));
        
        vertices.push_back(b2Vec2(x, y));
    }

    // Update position after placing the curve
    currentX += length; 
    currentY = startY + (heightAdjustment > 0 ? (inward ? -height : height) : (inward ? height : -height));

    return { vertices, startY, startY + (heightAdjustment > 0 ? (inward ? -height : height) : (inward ? height : -height)), false };
}


// Returns the x-coordinate of the finish line obstacle.
float getFinishLineX() {
    return finishLineX;
}
