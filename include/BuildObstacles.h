#ifndef BUILD_OBSTACLES_H
#define BUILD_OBSTACLES_H

#include <vector>
#include <Box2D/Box2D.h>
#include "Obstacle.h"

// Returns a set of obstacles forming:
// line(10) -> ramp(10) -> break marker (NaN,NaN) -> gap(5) -> break marker -> line(10)
std::vector<Obstacle> createSimpleRampSet();

// Represents a gap obstacle with a single vertex indicating gap length
Obstacle gap(float gapStartX, float gapLength, float height);

// Represents a break obstacle with a single (NaN, NaN) vertex
Obstacle breakObstacle();

#endif // BUILD_OBSTACLES_H
