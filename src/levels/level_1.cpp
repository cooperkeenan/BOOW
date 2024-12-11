#pragma once

#include <vector>
#include "Obstacle.h"

// Level 1 declarations
std::vector<Obstacle> level1_obstacle_1();
std::vector<Obstacle> level1_obstacle_2();
std::vector<Obstacle> level1_obstacle_3();
std::vector<Obstacle> level1_obstacle_4();
std::vector<Obstacle> level1_obstacle_5();
std::vector<Obstacle> level_1();

#include "levels.h"
#include "BuildObstacles.h"

// Obstacle 1
std::vector<Obstacle> level1_obstacle_1() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(line());
    obstacleSet.push_back(line());
    obstacleSet.push_back(ramp());
    obstacleSet.push_back(gap());
    obstacleSet.push_back(line(0, -5));
    return obstacleSet;
}

// Obstacle 2
std::vector<Obstacle> level1_obstacle_2() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(line());
    obstacleSet.push_back(ramp());
    obstacleSet.push_back(gap());
    obstacleSet.push_back(line(0, -5));
    return obstacleSet;
}

// Obstacle 3
std::vector<Obstacle> level1_obstacle_3() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(line());
    obstacleSet.push_back(ramp());
    obstacleSet.push_back(gap());
    obstacleSet.push_back(line(0, -5));
    return obstacleSet;
}

// Obstacle 4
std::vector<Obstacle> level1_obstacle_4() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(line());
    obstacleSet.push_back(ramp());
    obstacleSet.push_back(gap());
    obstacleSet.push_back(line(0, -5));
    return obstacleSet;
}

// Obstacle 5
std::vector<Obstacle> level1_obstacle_5() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(line());
    obstacleSet.push_back(ramp());
    obstacleSet.push_back(gap());
    obstacleSet.push_back(line(0, -5));
    return obstacleSet;
}

std::vector<Obstacle> level_1() {
    // Reset Variables
    currentX = INITIAL_START_X;
    currentY = INITIAL_START_Y;

    // Assemble Obstacles
    std::vector<Obstacle> level;
    auto part1 = level1_obstacle_1();
    auto part2 = level1_obstacle_2();
    auto part3 = level1_obstacle_3();
    auto part4 = level1_obstacle_4();
    auto part5 = level1_obstacle_5();

    // Concatenate Obstacles
    level.insert(level.end(), part1.begin(), part1.end());
    level.insert(level.end(), part2.begin(), part2.end());
    level.insert(level.end(), part3.begin(), part3.end());
    level.insert(level.end(), part4.begin(), part4.end());
    level.insert(level.end(), part5.begin(), part5.end());

    // Add large flat platform at the end
    level.push_back(largePlatform());

    return level;
}