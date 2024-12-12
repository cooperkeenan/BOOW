#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "GameState.h"
#include "PhysicsManager.h"
#include "Boat.h"
#include "AIController.h"
#include <vector>
#include "Obstacle.h"

// Manages the game menus and level selection.
class Menu {
public:
    Menu(sf::RenderWindow& window, sf::Font& font); // Constructor for the Menu class.
    void handleEvent(sf::Event& event, GameState& currentState, PhysicsManager& physicsManager, Boat& boat, Boat& secondBoat, float& timeRemaining, int& score, int& currentLevel); // Handles general menu events.
    void handleLevelCompleteEvent(sf::Event& event, GameState& currentState, PhysicsManager& physicsManager, Boat& boat, float& timeRemaining, int& score); // Handles events for the Level Complete screen.
    void draw(GameState currentState); // Draws the appropriate menu based on the current game state.
    void drawLevelCompleteScreen(LevelResult result); // Draws the Level Complete screen.
    void setLevelResult(LevelResult result); // Sets the result of the level (Complete or Failed).
    LevelResult getLevelResult() const; // Gets the result of the level.
    bool hasLevelChanged() const; // Checks if the player has selected a new level.
    void resetLevelChangedFlag(); // Resets the level changed flag.
    void handleEvent(sf::Event& event, GameState& currentState, PhysicsManager& physicsManager, Boat& boat, Boat& secondBoat, float& timeRemaining, int& score, int& currentLevel, AIController* aiController); //Handles general menu events with AIController
    void loadLevel(GameState& currentState, PhysicsManager& physicsManager, Boat& boat, Boat& secondBoat, float& timeRemaining, int& score, int currentLevel, const std::vector<Obstacle>& levelData); //Loads the selected level
private:
    sf::RenderWindow& window; // Reference to the game window.
    sf::Font& font;           // Reference to the font used for menu text.
    Button levelsButton;      // Button for navigating to the level selection menu.
    Button controlsButton;    // Button for navigating to the controls screen.
    Button quitButton;        // Button for quitting the game.
    Button level1Button;     // Button for selecting Level 1.
    Button level2Button;     // Button for selecting Level 2.
    Button level3Button;     // Button for selecting Level 3.
    LevelResult levelResult;  // Stores the result of the last played level.
    bool levelChanged;        // Flag indicating if the selected level has changed.
};