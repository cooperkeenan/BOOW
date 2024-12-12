#pragma once

// Global position tracking for obstacle creation.
extern float currentX;    // Current x-coordinate for drawing obstacles.
extern float currentY;    // Current y-coordinate for drawing obstacles.
extern int currentLevel; // Current level of the game.

// Constants for game window dimensions and Box2D scaling.
constexpr float WINDOW_WIDTH = 800.0f;  // Width of the game window.
constexpr float WINDOW_HEIGHT = 600.0f; // Height of the game window.
constexpr float SCALE = 10.0f;         // Scaling factor between Box2D units and SFML pixels.

// Collision categories for Box2D filtering. These are bit flags.
constexpr uint16_t CATEGORY_PLAYER = 0x0001;    // Category for the player boat.
constexpr uint16_t CATEGORY_AI = 0x0002;        // Category for AI boats.
constexpr uint16_t CATEGORY_OBSTACLE = 0x0004; // Category for obstacles.
constexpr uint16_t CATEGORY_COLLECTABLE = 0x0008; // Category for collectable items.