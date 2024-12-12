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

// Render the current game state (e.g. main menu, playing, paused, etc.)
void renderGameState(GameComponents& components, sf::RenderWindow& window) {
    clearWindow(window);

    // Switch based on the current game state and handle accordingly
    switch (components.currentState) {
        case GameState::MainMenu:
        case GameState::LevelSelection:
            // Draw main menu or level selection menus
            handleMainMenuOrLevelSelection(components, window);
            break;
        case GameState::Playing:
            // Draw and update objects while playing
            handlePlayingState(components, window);
            break;
        case GameState::Paused:
            // Draw the paused overlay and related UI
            handlePausedState(components, window);
            break;
        case GameState::Controls:
            // Draw the controls instructions
            handleControlsState(components, window);
            break;
        case GameState::LevelComplete:
            // Draw the level complete or failed screen
            handleLevelCompleteState(components, window);
            break;
    }

    // Display the current frame
    window.display();
}

// Clear the window before drawing the current frame
void clearWindow(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
}

// Handles the playing state logic: updates timers, player input, camera, and renders the scene
void handlePlayingState(GameComponents& components, sf::RenderWindow& window) {
    // If the timer is not paused, decrement the time remaining
    if (!components.timerPaused) {
        float elapsed = components.timerClock.restart().asSeconds();
        components.timeRemaining -= elapsed;

        // Check if time has run out
        if (components.timeRemaining <= 0.0f) {
            components.timeRemaining = 0.0f;
            components.currentState = GameState::LevelComplete;
            components.menu->setLevelResult(LevelResult::Failed);
        }
    }

    // Update UI elements: timer and score display
    components.timerText.setString("Time: " + std::to_string(static_cast<int>(components.timeRemaining)));
    components.scoreText.setString("Score: " + std::to_string(components.score));

    // Calculate deltaTime for physics and AI updates
    float deltaTime = components.clock.restart().asSeconds();
    

    // Player Controls: Check which keys are pressed and apply movement/rotation
    float directionX = 0.0f;
    float torque = 0.0f;

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


    // Apply the movement and rotation forces to the player's boat
    components.boat->move(directionX, 0.0f, 5.0f);
    components.boat->rotate(torque);


    // Update AI boat actions
    components.aiController->update(deltaTime);


    // Apply gravity to the world if conditions are met
    components.physicsManager->applyGravityIfNeeded(
        components.gravityApplied, components.clock.getElapsedTime().asSeconds(), 0.5f
    );


    // Advance the physics simulation by one time step
    components.physicsManager->step();


    // Update boat states (e.g., finish line checks)
    components.boat->update(components.currentState);
    components.secondBoat->update(components.currentState);


    // Smooth camera follow: lerp the view towards the player's boat position
    b2Vec2 boatPos = components.boat->getBoatBody()->GetPosition();
    sf::Vector2f targetCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
    sf::Vector2f currentCenter = components.gameView.getCenter();
    float lerpFactor = 0.1f;
    components.gameView.setCenter(currentCenter + lerpFactor * (targetCenter - currentCenter));


    // Check if the player boat needs to be respawned (e.g., fell off track)
    if (components.boat->checkRespawnNeeded()) {
        components.boat->respawnBoat(*components.physicsManager, components.currentLevel);
        components.secondBoat->respawnBoat(*components.physicsManager, components.currentLevel);
        components.gravityApplied = false;
        components.clock.restart();
        components.timeRemaining = 30.0f;
        components.timerClock.restart();
        components.score = 0;
    }

    // Check and update score based on any collected items
    components.score += components.physicsManager->checkCollectables();

    // Render the gameplay scene
    window.setView(components.gameView); // Set the game camera
    components.physicsManager->renderGround(window);
    components.physicsManager->renderCollectables(window);
    components.boat->render(window);
    components.secondBoat->render(window);

    // Render UI elements (timer, score) and the pause button on the default view
    window.setView(window.getDefaultView());
    window.draw(components.timerText);
    window.draw(components.scoreText);
    components.pauseButton->draw(window);
}

// Handle the paused state: draw the game scene static and overlay the pause menu
void handlePausedState(GameComponents& components, sf::RenderWindow& window) {
    window.setView(components.gameView);
    components.physicsManager->renderGround(window);
    components.physicsManager->renderCollectables(window);
    components.boat->render(window);
    window.setView(window.getDefaultView());
    components.pauseMenu->draw();
}

// Handle the controls state: display instructions and a back button
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

// Handle the level complete state: display success/failure screen and navigation options
void handleLevelCompleteState(GameComponents& components, sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    components.menu->drawLevelCompleteScreen(components.menu->getLevelResult());
}

// Handle the main menu or level selection states: just draw the menu appropriate for these states
void handleMainMenuOrLevelSelection(GameComponents& components, sf::RenderWindow& window) {
    components.menu->draw(components.currentState);
}
