#include "levels.h"
#include "BuildObstacles.h"

std::vector<Obstacle> level1_obstacle_1() {
    // Do not reset here. Assume currentX/currentY continue from outside.
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(startingLine());
    obstacleSet.push_back(line());
    obstacleSet.push_back(ramp());
    obstacleSet.push_back(gap());
    obstacleSet.push_back(line(0, -5));
    return obstacleSet;
}

std::vector<Obstacle> level1_obstacle_2() {
    // No reset here either, just continue from where obstacle_1 ended
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(line());
    obstacleSet.push_back(ramp());
    obstacleSet.push_back(gap());
    obstacleSet.push_back(line(0, -5));
    return obstacleSet;
}



std::vector<Obstacle> level_1() {
    // Reset at the start of the entire level
    currentX = INITIAL_START_X;
    currentY = INITIAL_START_Y;
    
    std::vector<Obstacle> level;
    auto part1 = level1_obstacle_1();
    auto part2 = level1_obstacle_2();

    // Concatenate the parts
    level.insert(level.end(), part1.begin(), part1.end());
    level.insert(level.end(), part2.begin(), part2.end());

    return level;
}
