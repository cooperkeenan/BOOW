#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "GameState.h"
#include "PhysicsManager.h"
#include "Boat.h"

class Menu {
public:
    Menu(sf::RenderWindow& window, sf::Font& font);
    void handleEvent(sf::Event& event, GameState& currentState);
    void handleLevelCompleteEvent(sf::Event& event, GameState& currentState, PhysicsManager& physicsManager, Boat& boat, float& timeRemaining, int& score);
    void draw(GameState currentState);
    void drawLevelCompleteScreen(LevelResult result);
    void setLevelResult(LevelResult result);
    LevelResult getLevelResult() const;  // Getter for levelResult


private:
    sf::RenderWindow& window;
    sf::Font& font;  // Store a reference to the font
    Button levelsButton;
    Button controlsButton;
    Button quitButton;
    Button level1Button;
    Button level2Button;
    Button level3Button;
    LevelResult levelResult;  // Add a member to track the result of the level
};
