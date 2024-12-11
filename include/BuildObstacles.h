#pragma once

#include <vector>
#include <Box2D/Box2D.h>
#include "Obstacle.h"

// Constants for standard dimensions
constexpr float INITIAL_START_X = -30.0f;
constexpr float INITIAL_START_Y = 30.0f;
constexpr float LINE_LENGTH = 5.0f;
constexpr float RAMP_LENGTH = 10.0f;
constexpr float RAMP_HEIGHT = 5.0f;
constexpr float TRIANGLE_SIDE = 5.0f; 
constexpr float GAP_LENGTH = 10.0f;

// Function Declarations
extern float currentX;
extern float currentY;

Obstacle line(float horizontalOffset = 0.0f, float verticalOffset = 0.0f, float lengthAdjustment = 0.0f);
Obstacle ramp(float horizontalOffset = 0.0f, float verticalOffset = 0.0f, float lengthAdjustment = 0.0f, float heightAdjustment = 0.0f);
Obstacle gap(float horizontalOffset = 0.0f, float lengthAdjustment = 0.0f);
Obstacle triangle(float horizontalOffset = 0.0f, float verticalOffset = 0.0f, float lengthAdjustment = 0.0f);
Obstacle finishLine(float horizontalOffset = 0.0f, float verticalOffset = 0.0f, float lengthAdjustment = 0.0f);
Obstacle triangle(float horizontalOffset, float verticalOffset, float lengthAdjustment, float rotationDegrees);
Obstacle curve(float horizontalOffset = 0.0f, float verticalOffset = 0.0f, float lengthAdjustment = 0.0f, float heightAdjustment = 0.0f); 

float getFinishLineX();