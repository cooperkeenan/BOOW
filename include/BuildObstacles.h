#ifndef BUILD_OBSTACLES_H
#define BUILD_OBSTACLES_H

#include <vector>
#include <Box2D/Box2D.h>
#include "Obstacle.h"

// Function to create a standard starting line
Obstacle startingLine(float startX, float startY, float length);

Obstacle line(float startX, float startY, float length);

// Function to create a ramp obstacle
Obstacle ramp(float startX, float startY, float length, float height);

// Function to create a gap obstacle with a single vertex indicating gap length
Obstacle gap(float gapStartX, float gapLength, float height);

// Function to create a complete set of obstacles for a level
std::vector<Obstacle> Obstacle_1();
std::vector<Obstacle> Obstacle_2();

#endif // BUILD_OBSTACLES_H
