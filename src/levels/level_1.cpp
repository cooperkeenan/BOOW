#include <vector>
#include "Obstacle.h"
#include "levels.h"
#include "BuildObstacles.h"

// Static variable to store the x-coordinate of the finish line for level 1.
static float finishLineX = 0;

// Creates the first set of obstacles for Level 1.
std::vector<Obstacle> level1_obstacle_1() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(line());
    obstacleSet.push_back(line());
    obstacleSet.push_back(ramp());
    obstacleSet.push_back(gap());
    obstacleSet.push_back(line(0, -5));
    return obstacleSet;
}

// Creates the second set of obstacles for Level 1.
std::vector<Obstacle> level1_obstacle_2() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(line());
    obstacleSet.push_back(ramp());
    obstacleSet.push_back(gap());
    obstacleSet.push_back(line(0, -5));
    return obstacleSet;
}

// Creates the third set of obstacles for Level 1.
std::vector<Obstacle> level1_obstacle_3() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(line());
    obstacleSet.push_back(ramp());
    obstacleSet.push_back(gap());
    obstacleSet.push_back(line(0, -5));
    return obstacleSet;
}

// Creates the fourth set of obstacles for Level 1.
std::vector<Obstacle> level1_obstacle_4() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(line());
    obstacleSet.push_back(ramp());
    obstacleSet.push_back(gap());
    obstacleSet.push_back(line(0, -5));
    return obstacleSet;
}

// Creates the fifth set of obstacles for Level 1.
std::vector<Obstacle> level1_obstacle_5() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(line());
    obstacleSet.push_back(ramp());
    obstacleSet.push_back(gap());
    obstacleSet.push_back(line(0, -5));
    return obstacleSet;
}

// Creates the finish line for Level 1.
std::vector<Obstacle> level1_finish_line() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(finishLine(0, 0, 10));
    return obstacleSet;
}

// Creates the complete Level 1 by combining all obstacle parts.
std::vector<Obstacle> level_1() {
    // Reset the global drawing position variables.
    currentX = INITIAL_START_X;
    currentY = INITIAL_START_Y;

    // Assemble the level by combining obstacle sets.
    std::vector<Obstacle> level;
    auto part1 = level1_obstacle_1();
    auto part2 = level1_obstacle_2();
    auto part3 = level1_obstacle_3();
    auto part4 = level1_obstacle_4();
    auto part5 = level1_obstacle_5();
    auto part6 = level1_finish_line();

    // Concatenate the obstacle sets into the final level vector.
    level.insert(level.end(), part1.begin(), part1.end());
    level.insert(level.end(), part2.begin(), part2.end());
    level.insert(level.end(), part3.begin(), part3.end());
    level.insert(level.end(), part4.begin(), part4.end());
    level.insert(level.end(), part5.begin(), part5.end());
    level.insert(level.end(), part6.begin(), part6.end());

    return level;
}