#pragma once

#include <SFML/Graphics.hpp>
#include "Boat.h"
#include "PhysicsManager.h"
#include "GameState.h"
#include "Menu.h"
#include "Pause.h"
#include "Button.h"
#include "AIController.h"
#include <vector>
#include <string>

// Structure to hold all core game components.
struct GameComponents {
    sf::Font font;                     // Font used for text rendering.
    Menu* menu;                       // Pointer to the main menu object.
    PhysicsManager* physicsManager;     // Pointer to the physics manager.
    Boat* boat;                       // Pointer to the player's boat.
    Boat* secondBoat;                 // Pointer to the second boat (AI controlled).
    sf::Clock clock;                    // Clock for general game timing.
    sf::Clock gravityClock;             // Clock for tracking gravity application timing.
    bool gravityApplied;               // Flag indicating if gravity has been applied to the player's boat.
    bool secondBoatGravityApplied;     // Flag indicating if gravity has been applied to the second boat.
    float lerpFactor;                  // Factor for camera smoothing (linear interpolation).
    GameState currentState;            // Current state of the game.
    GameState previousState;           // Previous state of the game.
    bool isReloaded = false;           // Flag to track if the game has been reloaded.
    sf::Clock timerClock;               // Clock for the game timer.
    float timeRemaining;              // Remaining game time.
    bool timerPaused;                 // Flag indicating if the timer is paused.
    sf::Text timerText;                // Text object for displaying the timer.
    int score;                         // Player's score.
    sf::Text scoreText;                // Text object for displaying the score.
    sf::View gameView;                 // View used for the game world (camera).
    Button* pauseButton;               // Pointer to the pause button.
    Pause* pauseMenu;                 // Pointer to the pause menu.
    std::vector<std::string> controlsText; // Text for the controls screen.
    Button* backButton;                // Pointer to the back button.
    std::vector<AIInput> aiInputs; // AI input sequence for the second boat
    AIController* aiController;       // AI controller for the second boat.
    int currentLevel;                  // The current level the player is on
    sf::Clock startDelayClock; // Clock for the start delay

};

// Function to initialize all game components.
GameComponents initializeGame(sf::RenderWindow& window);