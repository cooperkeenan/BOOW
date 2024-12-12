#include "BuildObstacles.h"
#include "Constants.h"
#include <cmath>
#include <algorithm>
#include <iostream>

// Global variables to track current drawing position.
float currentX = INITIAL_START_X; // Current x-coordinate for obstacle placement
float currentY = INITIAL_START_Y; // Current y-coordinate for obstacle placement
float finishLineX = 0.0f; // Declare global finishLineX variable


// Creates a line obstacle.
Obstacle line(float horizontalOffset, float verticalOffset, float lengthAdjustment) {
    currentX += horizontalOffset;                               // Apply horizontal offset
    float length = LINE_LENGTH + lengthAdjustment;                 // Calculate line length
    float startY = currentY + verticalOffset;                     // Calculate starting y-coordinate

    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(currentX, startY));               // Add starting vertex
    vertices.push_back(b2Vec2(currentX + length, startY));     // Add ending vertex
    currentX += length;                                       // Update current x-coordinate
    currentY = startY;                                         // Maintain current y-coordinate

    return {vertices, startY, startY, false};                   // Return the obstacle
}

// Creates a ramp obstacle.
Obstacle ramp(float horizontalOffset, float verticalOffset, float lengthAdjustment, float heightAdjustment) {
    currentX += horizontalOffset;                               // Apply horizontal offset
    float length = RAMP_LENGTH + lengthAdjustment;                 // Calculate ramp length
    float height = RAMP_HEIGHT + heightAdjustment;                 // Calculate ramp height
    float startY = currentY + verticalOffset;                     // Calculate starting y-coordinate

    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(currentX, startY));               // Add starting vertex
    vertices.push_back(b2Vec2(currentX + length, startY + height)); // Add ending vertex
    currentX += length;                                       // Update current x-coordinate
    currentY = startY + height;                                 // Update current y-coordinate

    return {vertices, startY, currentY, false};                   // Return the obstacle
}

// Creates a triangle obstacle with a specified rotation.
Obstacle triangle(float horizontalOffset, float verticalOffset, float lengthAdjustment, float rotationDegrees) {
    currentX += horizontalOffset;                               // Apply horizontal offset
    float side = TRIANGLE_SIDE + lengthAdjustment;            // Side length of the equilateral triangle
    float height = (std::sqrt(3.0f) / 2.0f) * side;         // Height of the equilateral triangle
    float startY = currentY + verticalOffset;                     // Calculate starting y-coordinate

    // Calculate triangle vertices relative to origin.
    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(0.0f, 0.0f));                   // Bottom-left corner
    vertices.push_back(b2Vec2(side, 0.0f));                   // Bottom-right corner
    vertices.push_back(b2Vec2(side / 2, height));             // Top vertex

    // Convert rotation from degrees to radians.
    float rotationRadians = rotationDegrees * (b2_pi / 180.0f);

    // Apply rotation to each vertex.
    for (auto& vertex : vertices) {
        float x = vertex.x;                                   // Store original x-coordinate
        float y = vertex.y;                                   // Store original y-coordinate

        vertex.x = x * std::cos(rotationRadians) - y * std::sin(rotationRadians); // Rotate x-coordinate
        vertex.y = x * std::sin(rotationRadians) + y * std::cos(rotationRadians); // Rotate y-coordinate

        // Translate to world position (currentX, startY).
        vertex.x += currentX;                               // Translate x-coordinate
        vertex.y += startY;                               // Translate y-coordinate
    }

    // Close the triangle by adding the first vertex again (optional but recommended).
    vertices.push_back(vertices[0]);

    // Update currentX to the farthest x-coordinate.
    currentX = std::max_element(vertices.begin(), vertices.end(), [](const b2Vec2& a, const b2Vec2& b) {
        return a.x < b.x;
    })->x;

    // Maintain currentY for continuity.
    currentY = startY;

    return {vertices, startY, startY + height, false};       // Return the obstacle
}

// Creates a gap obstacle.
Obstacle gap(float horizontalOffset, float lengthAdjustment) {
    currentX += horizontalOffset;                               // Apply horizontal offset
    float length = GAP_LENGTH + lengthAdjustment;                 // Calculate gap length

    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(currentX, currentY));               // Add starting vertex
    vertices.push_back(b2Vec2(currentX + length, currentY));     // Add ending vertex
    currentX += length;                                       // Update current x-coordinate

    return {vertices, currentY, currentY, true};                   // Return the obstacle
}

// Creates a finish line obstacle.
Obstacle finishLine(float horizontalOffset, float verticalOffset, float lengthAdjustment) {
    currentX += horizontalOffset;                               // Apply horizontal offset
    float length = LINE_LENGTH + lengthAdjustment;                 // Calculate line length
    float startY = currentY + verticalOffset;                     // Calculate starting y-coordinate

    // Set finish line position. Consider moving this logic to the Boat class.
    finishLineX = currentX;
    std::cout << "Finish Line created at X: " << finishLineX << std::endl; // Output finish line position for debugging

    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(currentX, startY));               // Add starting vertex
    vertices.push_back(b2Vec2(currentX + length, startY));     // Add ending vertex
    currentX += length;                                       // Update current x-coordinate
    currentY = startY;                                         // Maintain current y-coordinate

    return {vertices, startY, startY, false};                   // Return the obstacle
}

// Creates a curved obstacle.
Obstacle curve(float horizontalOffset, float verticalOffset, float lengthAdjustment, float heightAdjustment, bool inward) {
    currentX += horizontalOffset;                               // Apply horizontal offset
    float startY = currentY + verticalOffset;                     // Calculate starting y-coordinate
    float length = RAMP_LENGTH + lengthAdjustment;                 // Calculate curve length
    float height = RAMP_HEIGHT + std::abs(heightAdjustment); // Calculate curve height

    const int NUM_SEGMENTS = 20;                                // Number of segments to create the curve
    std::vector<b2Vec2> vertices;
    vertices.reserve(NUM_SEGMENTS + 1);                         // Reserve space for vertices

    for (int i = 0; i <= NUM_SEGMENTS; ++i) {
        float t = static_cast<float>(i) / NUM_SEGMENTS;         // Calculate interpolation factor (0.0 to 1.0)
        float x = currentX + t * length;                       // Calculate x-coordinate

        // Calculate y-value based on the 'inward' parameter.
        float y = startY + (inward ? (heightAdjustment > 0 ? -height * (1 - t) * (1 - t) : height * (1 - t) * (1 - t)) : (heightAdjustment > 0 ? height * t * t : -height * t * t));
        vertices.push_back(b2Vec2(x, y));                       // Add vertex to the curve
    }

    currentX += length;                                       // Update current x-coordinate
    currentY = startY + (heightAdjustment > 0 ? (inward ? -height : height) : (inward ? height : -height)); // Update current y-coordinate

    return {vertices, startY, startY + (heightAdjustment > 0 ? (inward ? -height : height) : (inward ? height : -height)), false}; // Return the obstacle
}

// Getter function for the finish line X position. Consider removing this as it is not needed
float getFinishLineX() {
    return finishLineX; // Return finish line x-coordinate
}