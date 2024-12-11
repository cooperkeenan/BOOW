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

// Helper function declarations
void handleMainMenuOrLevelSelection(GameComponents& components, sf::RenderWindow& window);
void handlePlayingState(GameComponents& components, sf::RenderWindow& window);
void handlePausedState(GameComponents& components, sf::RenderWindow& window);
void handleControlsState(GameComponents& components, sf::RenderWindow& window);
void handleLevelCompleteState(GameComponents& components, sf::RenderWindow& window);
void clearWindow(sf::RenderWindow& window);

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



// Function implementations
void clearWindow(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
}



void handleMainMenuOrLevelSelection(GameComponents& components, sf::RenderWindow& window) {
    components.menu->draw(components.currentState);
}



void handlePlayingState(GameComponents& components, sf::RenderWindow& window) {
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

    // Update camera position smoothly
    b2Vec2 boatPos = components.boat->getBoatBody()->GetPosition();
    sf::Vector2f targetCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
    sf::Vector2f currentCenter = components.gameView.getCenter();
    components.gameView.setCenter(currentCenter + components.lerpFactor * (targetCenter - currentCenter));

    // Handle player input
    float directionX = 0.0f;
    float torque = 0.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) directionX = 0.4f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) directionX = -0.4f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) torque = 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) torque = -1.0f;

    components.boat->move(directionX, 0.0f, 5.0f);
    components.boat->rotate(torque);

    components.physicsManager->applyGravityIfNeeded(
        components.gravityApplied, components.clock.getElapsedTime().asSeconds(), 0.5f);
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

    components.score += components.physicsManager->checkCollectables();

    components.physicsManager->renderGround(window);
    components.physicsManager->renderCollectables(window);
    components.boat->render(window);

    window.setView(window.getDefaultView());
    window.draw(components.timerText);
    window.draw(components.scoreText);
    components.pauseButton->draw(window);
}



void handlePausedState(GameComponents& components, sf::RenderWindow& window) {
    window.setView(components.gameView);
    components.physicsManager->renderGround(window);
    components.physicsManager->renderCollectables(window);
    components.boat->render(window);
    window.setView(window.getDefaultView());
    components.pauseMenu->draw();
}



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



void handleLevelCompleteState(GameComponents& components, sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    components.menu->drawLevelCompleteScreen(components.menu->getLevelResult());
}
