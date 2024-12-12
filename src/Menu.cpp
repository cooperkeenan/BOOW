#include <iostream>
#include "Menu.h"
#include "Constants.h"
#include "GameSetup.h"
#include "PhysicsManager.h"
#include "Boat.h"
#include "levels.h" 

// Constructor: Initializes all menu buttons and positions them on the screen.
Menu::Menu(sf::RenderWindow& window, sf::Font& font) 
    : window(window), font(font),
      levelsButton("Levels", {200, 50}, 20, sf::Color::Blue, sf::Color::White),
      quitButton("Quit", {200, 50}, 20, sf::Color::Red, sf::Color::White),
      level1Button("Level 1", {200, 50}, 20, sf::Color::Green, sf::Color::White),
      level2Button("Level 2", {200, 50}, 20, sf::Color::Green, sf::Color::White),
      level3Button("Level 3", {200, 50}, 20, sf::Color::Green, sf::Color::White),
      controlsButton("Controls", {200, 50}, 20, sf::Color::Yellow, sf::Color::White)
{
    // Set fonts and positions for main menu buttons
    levelsButton.setFont(font);
    levelsButton.setPosition({300, 200});

    quitButton.setFont(font);
    quitButton.setPosition({300, 300});

    // Set fonts and positions for level selection and controls buttons
    level1Button.setFont(font);
    level1Button.setPosition({300, 150});

    level2Button.setFont(font);
    level2Button.setPosition({300, 250});

    level3Button.setFont(font);
    level3Button.setPosition({300, 350});

    controlsButton.setFont(font);
    controlsButton.setPosition({300, 100});
}

// Handle events based on the current game state and mouse interactions.
void Menu::handleEvent(sf::Event& event, GameState& currentState, PhysicsManager& physicsManager, Boat& boat, Boat& secondBoat, float& timeRemaining, int& score, int& currentLevel) 
{
    // Check for left mouse clicks
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (currentState == GameState::MainMenu) {
            // Main Menu interactions
            if (quitButton.isMouseOver(window)) {
                window.close();
            } else if (levelsButton.isMouseOver(window)) {
                currentState = GameState::LevelSelection; // Go to level selection menu
            } else if (controlsButton.isMouseOver(window)) {
                currentState = GameState::Controls; // Show controls screen
            }
        } else if (currentState == GameState::LevelSelection) {
            // Level Selection interactions
            if (level1Button.isMouseOver(window)) {
                // Start Level 1
                currentState = GameState::Playing;
            } else if (level2Button.isMouseOver(window)) {
                // Start Level 2
                currentState = GameState::Playing;

                // Set the current level to 2 for game logic
                currentLevel = 2;

                // Reset physics and load Level 2 obstacles
                physicsManager.reset(level_2()); 

                // Respawn both player and AI boats at start positions for Level 2
                boat.respawnBoat(physicsManager, currentLevel);
                secondBoat.respawnBoat(physicsManager, currentLevel);

                // Reset timer and score for the new level
                timeRemaining = 30.0f;  
                score = 0;              
                std::cout << "Starting Level 2" << std::endl;
            } else if (level3Button.isMouseOver(window)) {
                // Future: Start Level 3 if implemented
            }
        }
    }
}

// Handle events specific to the LevelComplete state, allowing the user to return to the menu or quit.
void Menu::handleLevelCompleteEvent(sf::Event& event, GameState& currentState, PhysicsManager& physicsManager, Boat& boat, float& timeRemaining, int& score) {
    // Check for mouse clicks in the Level Complete screen
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (levelsButton.isMouseOver(window)) {
            // Return to Level Selection and reset state
            boat.respawnBoat(physicsManager, 1); // Assume going back to a default level or menu
            physicsManager.reset(level_1());
            timeRemaining = 30.0f;
            score = 0;

            currentState = GameState::LevelSelection;
        } else if (quitButton.isMouseOver(window)) {
            // Quit the application
            window.close();
        }
    }
}

// Draw the appropriate menu buttons depending on the current state.
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

// Draw the Level Complete screen, showing if the player succeeded or failed, and offering choices.
void Menu::drawLevelCompleteScreen(LevelResult result) {
    std::string displayText = (result == LevelResult::Complete && currentLevel) ? "Level Complete!" : "Level Failed!";

    sf::Text text(displayText, font, 50);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    sf::FloatRect textRect = text.getLocalBounds();

    // Center the text on the screen
    text.setOrigin(textRect.width / 2, textRect.height / 2);
    text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 3));
    window.draw(text);

    // Position and draw the Levels and Quit buttons to allow user navigation
    levelsButton.setPosition({300, 300});
    quitButton.setPosition({300, 400});
    levelsButton.draw(window);
    quitButton.draw(window);
}

// Set the result of the level (Complete or Failed) to be drawn later
void Menu::setLevelResult(LevelResult result) {
    levelResult = result;
}

// Get the current level result
LevelResult Menu::getLevelResult() const {
    return levelResult;
}
