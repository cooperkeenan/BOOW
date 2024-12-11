
#include <vector>
#include "Obstacle.h"
#include "levels.h"
#include "BuildObstacles.h"
#include <iostream>


// Level 2 declarations
std::vector<Obstacle> level2_obstacle_1();
std::vector<Obstacle> level2_obstacle_2();
std::vector<Obstacle> level3_obstacle_3();
std::vector<Obstacle> level4_obstacle_4();
std::vector<Obstacle> level5_obstacle_5();
std::vector<Obstacle> level5_obstacle_6();
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
    obstacleSet.push_back(gap(5));
    obstacleSet.push_back(curve(0, 0, 20, -10, false)); 
    obstacleSet.push_back(curve(5, -30, 20, -10, true));  
    obstacleSet.push_back(line(0, -15, 10));
    return obstacleSet;
}

// Obstacle 4
std::vector<Obstacle> level2_obstacle_4() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(ramp(0, 0, 20, 20));
    obstacleSet.push_back(gap(10));
    obstacleSet.push_back(triangle(0, -10, 5, 90));
    obstacleSet.push_back(gap(10));
    obstacleSet.push_back(triangle(0, -10, 5, 50));
    obstacleSet.push_back(gap(5));
    obstacleSet.push_back(triangle(0, -5, 5, 70));
    return obstacleSet;
}

// Obstacle 5
std::vector<Obstacle> level2_obstacle_5() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(gap(5));
    obstacleSet.push_back(line(0, 5, 10));
    obstacleSet.push_back(curve(0, 0, 20, 10, false));
    obstacleSet.push_back(gap(5));
    obstacleSet.push_back(curve(0, -15, 20, -10, true));
    obstacleSet.push_back(line(0, -15, 20));
    return obstacleSet;
}


// Obstacle 6
std::vector<Obstacle> level2_obstacle_6() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(curve(0, 0, 20, 15, false));
    obstacleSet.push_back(gap(5));
    obstacleSet.push_back(line(0, 0, 10));
    obstacleSet.push_back(gap(5));
    obstacleSet.push_back(curve(0, -5, 10, 2));
    obstacleSet.push_back(gap(5));
    obstacleSet.push_back(curve(0, 0, 10, 2, true));
    obstacleSet.push_back(gap(5));
    obstacleSet.push_back(curve(0, 0, 10, 2));
    return obstacleSet;
}


// Obstacle 7
std::vector<Obstacle> level2_obstacle_7() {
    std::vector<Obstacle> obstacleSet;
    obstacleSet.push_back(gap(5));
    obstacleSet.push_back(line(0, 0, 30));
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
    auto part4 = level2_obstacle_4();
    auto part5 = level2_obstacle_5();
    auto part6 = level2_obstacle_6();
    auto part7 = level2_obstacle_7();

    // Concatenate Obstacles
    level.insert(level.end(), part1.begin(), part1.end());
    level.insert(level.end(), part2.begin(), part2.end());
    level.insert(level.end(), part3.begin(), part3.end());
    level.insert(level.end(), part4.begin(), part4.end());
    level.insert(level.end(), part5.begin(), part5.end());
    level.insert(level.end(), part6.begin(), part6.end());
    level.insert(level.end(), part7.begin(), part7.end());

    return level;
}