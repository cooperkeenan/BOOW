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

// Struct to encapsulate all game components
struct GameComponents {
    sf::Font font;                      // Font for text
    Menu* menu;                         // Main menu
    PhysicsManager* physicsManager;     // Physics manager for the game
    Boat* boat;                         // Player boat object
    Boat* secondBoat;                   // Second boat object (AI-controlled)
    sf::Clock clock;                    // Clock for gameplay
    sf::Clock gravityClock;             // Clock for gravity timing
    bool gravityApplied;                // Flag for player gravity application
    bool secondBoatGravityApplied;      // Flag for second boat gravity application
    float lerpFactor;                   // Camera lerp factor
    GameState currentState;             // Current game state
    GameState previousState;            // Previous game state
    sf::Clock timerClock;               // Clock for timer
    float timeRemaining;                // Remaining time
    bool timerPaused;                   // Flag for timer pause
    sf::Text timerText;                 // Text for displaying timer
    int score;                          // Player's score
    sf::Text scoreText;                 // Text for displaying score
    sf::View gameView;                  // View for the game
    Button* pauseButton;                // Pause button
    Pause* pauseMenu;                   // Pause menu
    std::vector<std::string> controlsText; // Text for controls screen
    Button* backButton;                 // Back button
    std::vector<AIInput> aiInputs;      //Ai inputs
    AIController* aiController;         // AI controller for the second boat
};

// Function to initialize all game components
GameComponents initializeGame(sf::RenderWindow& window);
