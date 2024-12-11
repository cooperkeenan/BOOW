#include <iostream>
#include "Menu.h"
#include "Constants.h"
#include "GameSetup.h"
#include "PhysicsManager.h"
#include "Boat.h"
#include "levels.h" // Ensure this is included so level_2() is accessible

Menu::Menu(sf::RenderWindow& window, sf::Font& font) 
    : window(window), font(font),  
      levelsButton("Levels", {200, 50}, 20, sf::Color::Blue, sf::Color::Black),
      quitButton("Quit", {200, 50}, 20, sf::Color::Red, sf::Color::Black),
      level1Button("Level 1", {200, 50}, 20, sf::Color::Green, sf::Color::Black),
      level2Button("Level 2", {200, 50}, 20, sf::Color::Green, sf::Color::Black),
      level3Button("Level 3", {200, 50}, 20, sf::Color::Green, sf::Color::Black),
      controlsButton("Controls", {200, 50}, 20, sf::Color::Yellow, sf::Color::Black)
{
    levelsButton.setFont(font);
    levelsButton.setPosition({300, 200});

    quitButton.setFont(font);
    quitButton.setPosition({300, 300});

    level1Button.setFont(font);
    level1Button.setPosition({300, 150});

    level2Button.setFont(font);
    level2Button.setPosition({300, 250});

    level3Button.setFont(font);
    level3Button.setPosition({300, 350});

    controlsButton.setFont(font);
    controlsButton.setPosition({300, 100});
}

void Menu::handleEvent(sf::Event& event, GameState& currentState, PhysicsManager& physicsManager, Boat& boat, Boat& secondBoat, float& timeRemaining, int& score, int& currentLevel) 
{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (currentState == GameState::MainMenu) {
            if (quitButton.isMouseOver(window)) {
                window.close();
            } else if (levelsButton.isMouseOver(window)) {
                currentState = GameState::LevelSelection;
            } else if (controlsButton.isMouseOver(window)) {
                currentState = GameState::Controls;
            }
        } else if (currentState == GameState::LevelSelection) {
            if (level1Button.isMouseOver(window)) {
                currentState = GameState::Playing;
                // If you need to reset to level 1:
                // physicsManager.reset(level_1());
                // boat.respawnBoat(physicsManager, 1);
                // secondBoat.respawnBoat(physicsManager, 1);
                // timeRemaining = 30.0f;  
                // score = 0; 

                std::cout << "Starting Level 1" << std::endl;
            } else if (level2Button.isMouseOver(window)) {
                currentState = GameState::Playing;

                // Set the current level to 2
                currentLevel = 2;

                // Initialize Physics for Level 2
                physicsManager.reset(level_2()); 

                // Respawn both boats for level 2
                boat.respawnBoat(physicsManager, currentLevel);
                secondBoat.respawnBoat(physicsManager, currentLevel);

                // Reset timer and score
                timeRemaining = 30.0f;  
                score = 0;              

                std::cout << "Starting Level 2" << std::endl;
            } else if (level3Button.isMouseOver(window)) {
                std::cout << "Starting Level 3" << std::endl;
                // Implement logic for Level 3 if needed
            }
        }
    }
}

void Menu::handleLevelCompleteEvent(sf::Event& event, GameState& currentState, PhysicsManager& physicsManager, Boat& boat, float& timeRemaining, int& score) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (levelsButton.isMouseOver(window)) {
            // Change to Main Menu logic instead of LevelSelection
            currentState = GameState::MainMenu;

            // Reset game state
            boat.respawnBoat(physicsManager, 1); // Respawn the boat to its initial state
            physicsManager.reset(level_1());    // Reset physics (obstacles and collectables)
            timeRemaining = 30.0f;              // Reset timer
            score = 0;                          // Reset score
        } else if (quitButton.isMouseOver(window)) {
            window.close(); // Exit the game
        }
    }
}

void Menu::draw(GameState currentState) {
    // Draw only relevant buttons based on the current state
    if (currentState == GameState::MainMenu) {
        levelsButton.draw(window);
        controlsButton.draw(window);  // Draw the controls button
        quitButton.draw(window);
    } else if (currentState == GameState::LevelSelection) {
        level1Button.draw(window);
        level2Button.draw(window);
        level3Button.draw(window);
    }
}

void Menu::drawLevelCompleteScreen(LevelResult result) {
    std::string displayText = (result == LevelResult::Complete) ? "Level Complete!" : "Level Failed!";
    sf::Text text(displayText, font, 50);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.width / 2, textRect.height / 2);
    text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 3));
    window.draw(text);

    // Update "Levels" button text to "Main"
    levelsButton.setFont(font);
    levelsButton.setPosition({300, 300});
    levelsButton.setText("Main"); // Use the new label for the button

    quitButton.setPosition({300, 400});
    levelsButton.draw(window);
    quitButton.draw(window);
}

void Menu::setLevelResult(LevelResult result) {
    levelResult = result;
}

LevelResult Menu::getLevelResult() const {
    return levelResult;
}
