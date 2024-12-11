#include <SFML/Graphics.hpp>
#include "GameSetup.h"
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

            // Handle menu events
            if (components.currentState == GameState::LevelComplete) {
                components.menu->handleLevelCompleteEvent(event, components.currentState, *components.physicsManager, *components.boat, components.timeRemaining, components.score);
            } else {
                components.menu->handleEvent(event, components.currentState);
            }

            if (components.currentState == GameState::Playing) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (components.pauseButton->isMouseOver(window)) {
                        components.currentState = GameState::Paused;
                        components.timerPaused = true;
                    }
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

            // Handle Escape Key
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

        // Initialize camera and timer when switching to Playing
        if (components.previousState != components.currentState && components.currentState == GameState::Playing) {
            b2Vec2 boatPos = components.boat->getBoatBody()->GetPosition();
            sf::Vector2f initialCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
            components.gameView.setCenter(initialCenter);

            components.gravityApplied = false;
            components.clock.restart();

            if (components.previousState != GameState::Paused) {
                components.timeRemaining = 30.0f;
                components.timerClock.restart();
                components.timerPaused = false;
            }

            components.timerText.setString("Time: " + std::to_string(static_cast<int>(components.timeRemaining)));
        }

        components.previousState = components.currentState;

        window.setView(window.getDefaultView());
        window.clear(sf::Color::Black);

        if (components.currentState == GameState::MainMenu || components.currentState == GameState::LevelSelection) {
            components.menu->draw(components.currentState);
        } else if (components.currentState == GameState::Playing) {
            if (!components.timerPaused) {
                components.timeRemaining -= components.timerClock.restart().asSeconds();
                if (components.timeRemaining < 0.0f) {
                    components.timeRemaining = 0.0f;
                    components.currentState = GameState::LevelComplete;
                    components.menu->setLevelResult(LevelResult::Failed);
                }
            }

            components.timerText.setString("Time: " + std::to_string(static_cast<int>(components.timeRemaining)));
            components.scoreText.setString("Score: " + std::to_string(components.score));

            window.setView(components.gameView);

            // Handle player input
            float directionX = 0.0f;
            float torque = 0.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) directionX = 0.4f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) directionX = -0.4f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) torque = 1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) torque = -1.0f;

            components.boat->move(directionX, 0.0f, 5.0f);
            components.boat->rotate(torque);

            components.physicsManager->applyGravityIfNeeded(components.gravityApplied, components.clock.getElapsedTime().asSeconds(), 0.5f);
            components.physicsManager->step();
            components.boat->update(components.currentState);

            if (components.boat->checkRespawnNeeded()) {
                components.boat->respawnBoat(*components.physicsManager);
                components.gravityApplied = false;
                components.clock.restart();
                components.timeRemaining = 30.0f;
                components.timerClock.restart();
                components.score = 0;
            }

            // Smoothly move the view to follow the boat
            b2Vec2 boatPos = components.boat->getBoatBody()->GetPosition();
            sf::Vector2f targetCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
            sf::Vector2f currentCenter = components.gameView.getCenter();
            components.gameView.setCenter(currentCenter + components.lerpFactor * (targetCenter - currentCenter));

            components.score += components.physicsManager->checkCollectables();

            components.physicsManager->renderGround(window);
            components.physicsManager->renderCollectables(window);
            components.boat->render(window);

            window.setView(window.getDefaultView());
            window.draw(components.timerText);
            window.draw(components.scoreText);
            components.pauseButton->draw(window);

        } else if (components.currentState == GameState::Paused) {
            window.setView(components.gameView);
            components.physicsManager->renderGround(window);
            components.physicsManager->renderCollectables(window);
            components.boat->render(window);
            window.setView(window.getDefaultView());
            components.pauseMenu->draw();
        } else if (components.currentState == GameState::Controls) {
            float yOffset = 100.0f;
            for (const auto& line : components.controlsText) {
                sf::Text text(line, components.font, 20);
                text.setFillColor(sf::Color::White);
                text.setPosition(50, yOffset);
                yOffset += 50;
                window.draw(text);
            }
            components.backButton->draw(window);
        } else if (components.currentState == GameState::LevelComplete) {
            window.clear(sf::Color::Black);
            components.menu->drawLevelCompleteScreen(components.menu->getLevelResult());
        }

        window.display();
    }

    return 0;
}
