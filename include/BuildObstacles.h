#ifndef BUILD_OBSTACLES_H
#define BUILD_OBSTACLES_H

#include <vector>
#include <Box2D/Box2D.h>
#include "Obstacle.h"

// Constants for standard dimensions
constexpr float INITIAL_START_X = -30.0f;
constexpr float INITIAL_START_Y = 30.0f;
constexpr float LINE_LENGTH = 5.0f;
constexpr float RAMP_LENGTH = 10.0f;
constexpr float RAMP_HEIGHT = 5.0f;
constexpr float GAP_LENGTH = 10.0f;

// In BuildObstacles.h
extern float currentX;
extern float currentY;


//Goes at start of each level
Obstacle startingLine();

//Line 
Obstacle line(float horizontalOffset = 0.0f, float verticalOffset = 0.0f, float lengthAdjustment = 0.0f);

//ramp
Obstacle ramp(float horizontalOffset = 0.0f, float verticalOffset = 0.0f, float lengthAdjustment = 0.0f, float heightAdjustment = 0.0f);

//Gap
Obstacle gap(float horizontalOffset = 0.0f, float lengthAdjustment = 0.0f);

#endif // BUILD_OBSTACLES_H
