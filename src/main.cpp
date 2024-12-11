#include <SFML/Graphics.hpp>
#include "GameSetup.h"
#include "RenderGame.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Boat Out of Water");

    // Initialize game components
    GameComponents components = initializeGame(window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle menu and game state events
            if (components.currentState == GameState::LevelComplete) {
                components.menu->handleLevelCompleteEvent(event, components.currentState, *components.physicsManager, *components.boat, components.timeRemaining, components.score);
            } else {
                components.menu->handleEvent(event, components.currentState);
            }

            // Handle input for pause and control states
            if (components.currentState == GameState::Playing && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (components.pauseButton->isMouseOver(window)) {
                    components.currentState = GameState::Paused;
                    components.timerPaused = true;
                }
            } else if (components.currentState == GameState::Paused) {
                components.pauseMenu->handleEvent(event, components.currentState);
                if (components.currentState == GameState::MainMenu) {
                    components.boat->respawnBoat(*components.physicsManager);
                    components.timeRemaining = 30.0f;
                    components.timerPaused = true;
                }
            } else if (components.currentState == GameState::Controls) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (components.backButton->isMouseOver(window)) {
                        components.currentState = GameState::MainMenu;
                        components.boat->respawnBoat(*components.physicsManager);
                        components.timeRemaining = 30.0f;
                        components.timerPaused = true;
                    }
                }
            }

            // Escape key handling for pause and resume
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                if (components.currentState == GameState::Playing) {
                    components.currentState = GameState::Paused;
                    components.timerPaused = true;
                } else if (components.currentState == GameState::Paused) {
                    components.currentState = GameState::Playing;
                    components.timerPaused = false;
                }
            }
        }

        // Delegate rendering and updates to the renderGameState function
        renderGameState(components, window);
    }

    return 0;
}
