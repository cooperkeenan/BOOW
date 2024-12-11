#include "EventHandler.h"

// Helper function declarations
void handleLevelCompleteEvent(sf::Event& event, GameComponents& components);
void handleMenuEvent(sf::Event& event, GameComponents& components);
void handlePlayingStateEvent(sf::RenderWindow& window, sf::Event& event, GameComponents& components);
void handlePausedStateEvent(sf::Event& event, GameComponents& components);
void handleControlsStateEvent(sf::RenderWindow& window, sf::Event& event, GameComponents& components);
void handleEscapeKeyEvent(sf::Event& event, GameComponents& components);

// Main function to handle all game events
void handleGameEvents(sf::RenderWindow& window, sf::Event& event, GameComponents& components) {
    if (components.currentState == GameState::LevelComplete) {
        handleLevelCompleteEvent(event, components);
    } else {
        handleMenuEvent(event, components);
    }

    switch (components.currentState) {
        case GameState::Playing:
            handlePlayingStateEvent(window, event, components);
            break;
        case GameState::Paused:
            handlePausedStateEvent(event, components);
            break;
        case GameState::Controls:
            handleControlsStateEvent(window, event, components);
            break;
        default:
            break;
    }

    handleEscapeKeyEvent(event, components);
}

// Handle events in Level Complete state
void handleLevelCompleteEvent(sf::Event& event, GameComponents& components) {
    components.menu->handleLevelCompleteEvent(event, components.currentState, *components.physicsManager, *components.boat, components.timeRemaining, components.score);
}

// Handle menu-related events
void handleMenuEvent(sf::Event& event, GameComponents& components) {
    components.menu->handleEvent(event, components.currentState);
}

// Handle events in the Playing state
void handlePlayingStateEvent(sf::RenderWindow& window, sf::Event& event, GameComponents& components) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (components.pauseButton->isMouseOver(window)) {
            components.currentState = GameState::Paused;
            components.timerPaused = true;
        }
    }
}

// Handle events in the Paused state
void handlePausedStateEvent(sf::Event& event, GameComponents& components) {
    components.pauseMenu->handleEvent(event, components.currentState);
    if (components.currentState == GameState::MainMenu) {
        components.boat->respawnBoat(*components.physicsManager);
        components.timeRemaining = 30.0f;
        components.timerPaused = true;
    }
}

// Handle events in the Controls state
void handleControlsStateEvent(sf::RenderWindow& window, sf::Event& event, GameComponents& components) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (components.backButton->isMouseOver(window)) {
            components.currentState = GameState::MainMenu;
            components.boat->respawnBoat(*components.physicsManager);
            components.timeRemaining = 30.0f;
            components.timerPaused = true;
        }
    }
}

// Handle Escape key for pause and resume
void handleEscapeKeyEvent(sf::Event& event, GameComponents& components) {
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
