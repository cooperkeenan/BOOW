#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Obstacle.h"

// Function declarations for creating the obstacles in Level 1.
std::vector<Obstacle> level1_obstacle_1(); // Creates the first set of obstacles for Level 1.
std::vector<Obstacle> level1_obstacle_2(); // Creates the second set of obstacles for Level 1.
std::vector<Obstacle> level1_obstacle_3(); // Creates the third set of obstacles for Level 1.
std::vector<Obstacle> level1_obstacle_4(); // Creates the fourth set of obstacles for Level 1.
std::vector<Obstacle> level1_obstacle_5(); // Creates the fifth set of obstacles for Level 1.
std::vector<Obstacle> level1_finish_line(); // Creates the finish line for Level 1.
std::vector<Obstacle> level_1();           // Creates the complete Level 1 by combining all obstacle parts.

// Function declarations for creating the obstacles in Level 2.
std::vector<Obstacle> level2_obstacle_1(); // Creates the first set of obstacles for Level 2.
std::vector<Obstacle> level2_obstacle_2(); // Creates the second set of obstacles for Level 2.
std::vector<Obstacle> level2_obstacle_3(); // Creates the third set of obstacles for Level 2.
std::vector<Obstacle> level2_obstacle_4(); // Creates the fourth set of obstacles for Level 2.
std::vector<Obstacle> level2_obstacle_5(); // Creates the fifth set of obstacles for Level 2.
std::vector<Obstacle> level2_finish_line();// Creates the finish line for Level 2.
std::vector<Obstacle> level_2();           // Creates the complete Level 2 by combining all obstacle parts.