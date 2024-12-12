#include <iostream>
#include "Menu.h"
#include "Constants.h"
#include "GameSetup.h"
#include "PhysicsManager.h"
#include "Boat.h"
#include "levels.h"
#include "AIController.h"

// Constructor for the Menu class. Initializes buttons and sets their positions and fonts.
Menu::Menu(sf::RenderWindow& window, sf::Font& font)
    : window(window), font(font),
      levelsButton("Levels", {200, 50}, 20, sf::Color::Blue, sf::Color::Black),
      quitButton("Quit", {200, 50}, 20, sf::Color::Red, sf::Color::Black),
      level1Button("Level 1", {200, 50}, 20, sf::Color::Green, sf::Color::Black),
      level2Button("Level 2", {200, 50}, 20, sf::Color::Green, sf::Color::Black),
      level3Button("Level 3", {200, 50}, 20, sf::Color::Green, sf::Color::Black),
      controlsButton("Controls", {200, 50}, 20, sf::Color::Yellow, sf::Color::Black) {

    levelsButton.setFont(font);       // Set the font for the Levels button.
    levelsButton.setPosition({300, 200}); // Set the position of the Levels button.

    quitButton.setFont(font);         // Set the font for the Quit button.
    quitButton.setPosition({300, 300});   // Set the position of the Quit button.

    level1Button.setFont(font);       // Set the font for the Level 1 button.
    level1Button.setPosition({300, 150}); // Set the position of the Level 1 button.

    level2Button.setFont(font);       // Set the font for the Level 2 button.
    level2Button.setPosition({300, 250}); // Set the position of the Level 2 button.

    level3Button.setFont(font);       // Set the font for the Level 3 button.
    level3Button.setPosition({300, 350}); // Set the position of the Level 3 button.

    controlsButton.setFont(font);     // Set the font for the Controls button.
    controlsButton.setPosition({300, 100}); // Set the position of the Controls button.
}

// Handles events for the menu, including button clicks.
void Menu::handleEvent(sf::Event& event, GameState& currentState, PhysicsManager& physicsManager, Boat& boat, Boat& secondBoat, float& timeRemaining, int& score, int& currentLevel) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (currentState == GameState::MainMenu) {
            if (quitButton.isMouseOver(window)) {
                window.close(); // Close the window if the Quit button is clicked.
            } else if (levelsButton.isMouseOver(window)) {
                currentState = GameState::LevelSelection; // Switch to the Level Selection state.
            } else if (controlsButton.isMouseOver(window)) {
                currentState = GameState::Controls; // Switch to the Controls state.
            }
        } else if (currentState == GameState::LevelSelection) {
            if (level1Button.isMouseOver(window)) {
                currentLevel = 1; // Set current level
                loadLevel(currentState, physicsManager, boat, secondBoat, timeRemaining, score, currentLevel, level_1()); // Load level data
                std::cout << "Starting Level 1" << std::endl;
                currentState = GameState::Playing; // Switch to the Playing state.
            } else if (level2Button.isMouseOver(window)) {
                currentLevel = 2; // Set current level

                // Reset physics for the new level
                physicsManager.reset(level_2());

                // Respawn boats for the new level
                boat.respawnBoat(physicsManager, currentLevel);
                secondBoat.respawnBoat(physicsManager, currentLevel);

                // Reset timer and score
                timeRemaining = 30.0f;
                score = 0;
                loadLevel(currentState, physicsManager, boat, secondBoat, timeRemaining, score, currentLevel, level_2());
                std::cout << "Starting Level 2" << std::endl;
                currentState = GameState::Playing;

            } else if (level3Button.isMouseOver(window)) {
                //Handle level 3
            }
        }
    }
}

// Handles events specific to the Level Complete screen.
void Menu::handleLevelCompleteEvent(sf::Event& event, GameState& currentState, PhysicsManager& physicsManager, Boat& boat, float& timeRemaining, int& score) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (levelsButton.isMouseOver(window)) {
            currentState = GameState::MainMenu; // Go back to Main Menu

            boat.respawnBoat(physicsManager, 1); // Respawn boat
            physicsManager.reset(level_1()); // Reset physics
            timeRemaining = 30.0f; // Reset timer
            score = 0; // Reset score
        } else if (quitButton.isMouseOver(window)) {
            window.close(); // Close the window
        }
    }
}

// Returns true if the level has changed, false otherwise.
bool Menu::hasLevelChanged() const {
    return levelChanged;
}

// Resets the level changed flag.
void Menu::resetLevelChangedFlag() {
    levelChanged = false;
}

// Draws the appropriate menu elements based on the current game state.
void Menu::draw(GameState currentState) {
    if (currentState == GameState::MainMenu) {
        levelsButton.draw(window);
        controlsButton.draw(window);
        quitButton.draw(window);
    } else if (currentState == GameState::LevelSelection) {
        level1Button.draw(window);
        level2Button.draw(window);
        level3Button.draw(window);
    }
}

// Draws the Level Complete screen with the appropriate message.
void Menu::drawLevelCompleteScreen(LevelResult result) {
    std::string displayText = (result == LevelResult::Complete) ? "Level Complete!" : "Level Failed!";
    sf::Text text(displayText, font, 50);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.width / 2, textRect.height / 2);
    text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 3));
    window.draw(text);

    levelsButton.setFont(font);
    levelsButton.setPosition({300, 300});
    quitButton.setPosition({300, 400});
    levelsButton.draw(window);
    quitButton.draw(window);
}

// Sets the level result.
void Menu::setLevelResult(LevelResult result) {
    levelResult = result;
}

// Gets the level result.
LevelResult Menu::getLevelResult() const {
    return levelResult;
}

// Loads the specified level and sets the game state to Playing.
void Menu::loadLevel(GameState& currentState, PhysicsManager& physicsManager, Boat& boat, Boat& secondBoat, float& timeRemaining, int& score, int currentLevel, const std::vector<Obstacle>& levelData) {
    currentState = GameState::Playing; // Set the game state to Playing.
    physicsManager.reset(levelData); // Reset the physics world with the new level data.
    boat.respawnBoat(physicsManager, currentLevel); // Respawn the player's boat.
    secondBoat.respawnBoat(physicsManager, currentLevel); // Respawn the AI boat.
    timeRemaining = 30.0f; // Reset the timer.
    score = 0; // Reset the score.
}