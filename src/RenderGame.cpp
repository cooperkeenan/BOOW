#include "GameSetup.h"
#include "RenderGame.h"
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "PhysicsManager.h"
#include "Boat.h"
#include "Pause.h"
#include "Button.h"
#include "Constants.h"
#include <vector>
#include <iostream>
#include <string>

// Function to render the game state based on the current state of the game
void renderGameState(GameComponents& components, sf::RenderWindow& window) {
    clearWindow(window);

    switch (components.currentState) {
        case GameState::MainMenu:
        case GameState::LevelSelection:
            handleMainMenuOrLevelSelection(components, window);
            break;
        case GameState::Playing:
            handlePlayingState(components, window);
            break;
        case GameState::Paused:
            handlePausedState(components, window);
            break;
        case GameState::Controls:
            handleControlsState(components, window);
            break;
        case GameState::LevelComplete:
            handleLevelCompleteState(components, window);
            break;
    }

    window.display();
}

// Function to clear the game window
void clearWindow(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
}

// Handle Main Menu or Level Selection states
void handlePlayingState(GameComponents& components, sf::RenderWindow& window) {
    if (!components.timerPaused) {
        // Update timer
        float elapsed = components.timerClock.restart().asSeconds();
        components.timeRemaining -= elapsed;

        if (components.timeRemaining <= 0.0f) {
            components.timeRemaining = 0.0f;
            components.currentState = GameState::LevelComplete;
            components.menu->setLevelResult(LevelResult::Failed);
        }
    }

    // Update timer and score texts
    components.timerText.setString("Time: " + std::to_string(static_cast<int>(components.timeRemaining)));
    components.scoreText.setString("Score: " + std::to_string(components.score));

    // Calculate deltaTime for this frame
    float deltaTime = components.clock.restart().asSeconds();

    // **Player Controls** (Re-added)
    float directionX = 0.0f;
    float torque = 0.0f;

    // Debugging: Verify input detection
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {

        directionX = 0.7f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {

        directionX = -0.7f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {

        torque = 2.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {

        torque = -2.0f;
    }


    components.boat->move(directionX, 0.0f, 5.0f); // Apply linear force
    components.boat->rotate(torque);              // Apply torque

    // **Physics and AI Updates**
    components.aiController->update(deltaTime); // Update AI Controller

    // Apply gravity
    components.physicsManager->applyGravityIfNeeded(
        components.gravityApplied, components.clock.getElapsedTime().asSeconds(), 0.5f
    );

    // Step the physics world
    components.physicsManager->step();

    // Update boats
    components.boat->update(components.currentState);
    components.secondBoat->update(components.currentState);

    // Smooth Camera Follow Logic
    b2Vec2 boatPos = components.boat->getBoatBody()->GetPosition();
    sf::Vector2f targetCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
    sf::Vector2f currentCenter = components.gameView.getCenter();

    // Smooth camera movement using lerp
    float lerpFactor = 0.1f; // Adjust this for smoother movement
    components.gameView.setCenter(currentCenter + lerpFactor * (targetCenter - currentCenter));

    // Check for respawn
    if (components.boat->checkRespawnNeeded()) {
        components.boat->respawnBoat(*components.physicsManager);
        components.secondBoat->respawnBoat(*components.physicsManager);

        components.gravityApplied = false;
        components.clock.restart();
        components.timeRemaining = 30.0f;
        components.timerClock.restart();
        components.score = 0;
    }

    // Update score
    components.score += components.physicsManager->checkCollectables();

    // Render game objects
    window.setView(components.gameView); // Apply the game view for rendering
    components.physicsManager->renderGround(window);
    components.physicsManager->renderCollectables(window);
    components.boat->render(window);
    components.secondBoat->render(window);

    // Render UI elements
    window.setView(window.getDefaultView());
    window.draw(components.timerText);
    window.draw(components.scoreText);
    components.pauseButton->draw(window);
}



// Remaining functions: Paused, Controls, and LevelComplete states





// Paused state
void handlePausedState(GameComponents& components, sf::RenderWindow& window) {
    window.setView(components.gameView);
    components.physicsManager->renderGround(window);
    components.physicsManager->renderCollectables(window);
    components.boat->render(window);
    window.setView(window.getDefaultView());
    components.pauseMenu->draw();
}


// Controls state
void handleControlsState(GameComponents& components, sf::RenderWindow& window) {
    float yOffset = 100.0f;
    for (const auto& line : components.controlsText) {
        sf::Text text(line, components.font, 20);
        text.setFillColor(sf::Color::White);
        text.setPosition(50, yOffset);
        yOffset += 50;
        window.draw(text);
    }
    components.backButton->draw(window);
}


// Complete state
void handleLevelCompleteState(GameComponents& components, sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    components.menu->drawLevelCompleteScreen(components.menu->getLevelResult());
}

void handleMainMenuOrLevelSelection(GameComponents& components, sf::RenderWindow& window) {
    components.menu->draw(components.currentState);
}
