
#include <vector>
#include "Obstacle.h"
#include "levels.h"
#include "BuildObstacles.h"


// Level 1 declarations
std::vector<Obstacle> level2_obstacle_1();
std::vector<Obstacle> level2_obstacle_2();

std::vector<Obstacle> level_1();




static float finishLineX = 0;


// Obstacle 1
std::vector<Obstacle> level2_obstacle_1() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(line());
    obstacleSet.push_back(line());
    obstacleSet.push_back(ramp());
    obstacleSet.push_back(gap());
    obstacleSet.push_back(line(0, -5));
    return obstacleSet;
}


// Obstacle 5
std::vector<Obstacle> level2_obstacle_2() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(triangle(0, 0, 5));
    obstacleSet.push_back(triangle(0, 0, 5));
   
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

    // Concatenate Obstacles
    level.insert(level.end(), part1.begin(), part1.end());
    level.insert(level.end(), part2.begin(), part2.end());

    return level;
}