#pragma once

#include <vector>
#include <Box2D/Box2D.h>
#include "Obstacle.h"

// Constants defining standard dimensions for obstacle creation.
constexpr float INITIAL_START_X = -30.0f;      // Initial x-coordinate for obstacle placement.
constexpr float INITIAL_START_Y = 30.0f;      // Initial y-coordinate for obstacle placement.
constexpr float LINE_LENGTH = 5.0f;           // Standard length for lines.
constexpr float RAMP_LENGTH = 10.0f;          // Standard length for ramps.
constexpr float RAMP_HEIGHT = 5.0f;          // Standard height for ramps.
constexpr float TRIANGLE_SIDE = 5.0f;       // Standard side length for triangles.
constexpr float GAP_LENGTH = 10.0f;          // Standard length for gaps.
constexpr float PARTIAL_CIRCLE_RADIUS = 7.0f; // Standard radius for partial circles (curves).
constexpr float PARTIAL_CIRCLE_ANGLE = 270.0f;// Standard angle for partial circles.

// Global variables to track the current drawing position (used in obstacle creation).
extern float currentX;
extern float currentY;

// Function declarations for creating different types of obstacles.
Obstacle line(float horizontalOffset = 0.0f, float verticalOffset = 0.0f, float lengthAdjustment = 0.0f); // Creates a line segment.
Obstacle ramp(float horizontalOffset = 0.0f, float verticalOffset = 0.0f, float lengthAdjustment = 0.0f, float heightAdjustment = 0.0f); // Creates a ramp.
Obstacle gap(float horizontalOffset = 0.0f, float lengthAdjustment = 0.0f); // Creates a gap.
Obstacle triangle(float horizontalOffset = 0.0f, float verticalOffset = 0.0f, float lengthAdjustment = 0.0f); // Creates a triangle.
Obstacle finishLine(float horizontalOffset = 0.0f, float verticalOffset = 0.0f, float lengthAdjustment = 0.0f); // Creates a finish line segment.
Obstacle triangle(float horizontalOffset, float verticalOffset, float lengthAdjustment, float rotationDegrees); // Creates a triangle with a specific rotation.
Obstacle curve(float horizontalOffset, float verticalOffset, float lengthAdjustment, float heightAdjustment, bool inward = false); // Creates a curved segment.

// Function to get the x-coordinate of the finish line.
float getFinishLineX();