#ifndef BUILD_OBSTACLES_H
#define BUILD_OBSTACLES_H

#include <vector>
#include <Box2D/Box2D.h>
#include "Obstacle.h"

// Returns two obstacles forming:
// line(10) -> ramp(10) -> gap(5) -> line(10)
std::vector<Obstacle> createSimpleRampSet();

// Represents a gap with a single vertex indicating the gap length
Obstacle gap(float gapLength);

#endif // BUILD_OBSTACLES_H
