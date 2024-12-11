// Menu.h
#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "GameState.h"
#include "PhysicsManager.h"
#include "Boat.h"
#include "AIController.h"
#include <vector>
#include "Obstacle.h"

class Menu {
public:
    Menu(sf::RenderWindow& window, sf::Font& font);
    void handleEvent(sf::Event& event, GameState& currentState, PhysicsManager& physicsManager, Boat& boat, Boat& secondBoat, float& timeRemaining, int& score, int& currentLevel);
    void handleLevelCompleteEvent(sf::Event& event, GameState& currentState, PhysicsManager& physicsManager, Boat& boat, float& timeRemaining, int& score);
    void draw(GameState currentState);
    void drawLevelCompleteScreen(LevelResult result);
    void setLevelResult(LevelResult result);
    LevelResult getLevelResult() const;

    void handleEvent(sf::Event& event, GameState& currentState, PhysicsManager& physicsManager, Boat& boat, Boat& secondBoat, float& timeRemaining, int& score, int& currentLevel, AIController* aiController);
    void loadLevel(GameState& currentState, PhysicsManager& physicsManager, Boat& boat, Boat& secondBoat, float& timeRemaining, int& score, int currentLevel, const std::vector<Obstacle>& levelData);
private:
    sf::RenderWindow& window;
    sf::Font& font;
    Button levelsButton;
    Button controlsButton;
    Button quitButton;
    Button level1Button;
    Button level2Button;
    Button level3Button;
    LevelResult levelResult;
};