#pragma once

// Global position tracking
extern float currentX;
extern float currentY;
extern int currentLevel;


// Constants for game dimensions and physics
constexpr float WINDOW_WIDTH = 800.0f;
constexpr float WINDOW_HEIGHT = 600.0f;
constexpr float SCALE = 10.0f;


// Collision categories
constexpr uint16_t CATEGORY_PLAYER = 0x0001;
constexpr uint16_t CATEGORY_AI = 0x0002;
constexpr uint16_t CATEGORY_OBSTACLE = 0x0004;
constexpr uint16_t CATEGORY_COLLECTABLE = 0x0008; // Unique bit for collectables
