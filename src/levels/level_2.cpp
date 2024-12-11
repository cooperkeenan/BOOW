
#include <vector>
#include "Obstacle.h"
#include "levels.h"
#include "BuildObstacles.h"
#include <iostream>


// Level 2 declarations
std::vector<Obstacle> level2_obstacle_1();
std::vector<Obstacle> level2_obstacle_2();
std::vector<Obstacle> level3_obstacle_3();
std::vector<Obstacle> level_1();


static float finishLineX = 0;


// Obstacle 1
std::vector<Obstacle> level2_obstacle_1() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(line(0,0,15));
    obstacleSet.push_back(line());
    obstacleSet.push_back(ramp());
    obstacleSet.push_back(gap());
    return obstacleSet;
}


// Obstacle 2
std::vector<Obstacle> level2_obstacle_2() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(triangle(0, 0, 5, 90));
    obstacleSet.push_back(line(0, -5, 40)); 
    return obstacleSet;
}


// Obstacle 3
std::vector<Obstacle> level2_obstacle_3() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(curve(0,0, 20, 15));
    obstacleSet.push_back(gap());
    obstacleSet.push_back(curve(0, 0, 20, -10, false)); // Downward outward curve (convex)
    obstacleSet.push_back(curve(5, -30, 20, -10, true));  // Downward inward curve (concave)

    return obstacleSet;
}




std::vector<Obstacle> level_2() {
    // Reset Variables
    currentX = INITIAL_START_X;
    currentY = INITIAL_START_Y;

    // Assemble Obstacles
    std::vector<Obstacle> level;
    auto part1 = level2_obstacle_1();
    auto part2 = level2_obstacle_2();
    auto part3 = level2_obstacle_3();

    // Concatenate Obstacles
    level.insert(level.end(), part1.begin(), part1.end());
    level.insert(level.end(), part2.begin(), part2.end());
    level.insert(level.end(), part3.begin(), part3.end());

    return level;
}