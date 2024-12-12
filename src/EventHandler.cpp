#include "EventHandler.h"
#include "GameSetup.h"
#include "levels.h"


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
    } else if (components.currentState == GameState::MainMenu || components.currentState == GameState::LevelSelection) {
        handleMenuEvent(window, event, components); // Correct: Passes window
    }

    switch (components.currentState) {
        case GameState::Playing:
            // Ensure proper timer setup on state transition
            if (components.previousState != components.currentState) {
                // If coming from other states, reset timer and apply delay
                if (components.previousState == GameState::MainMenu || 
                    components.previousState == GameState::LevelSelection || 
                    components.isReloaded) {
                    components.timeRemaining = 30.0f; // Reset the timer
                    components.timerPaused = true;   // Temporarily pause the timer
                    components.startDelayClock.restart(); // Restart the delay clock
                    components.isReloaded = false;  // Reset reload flag
                }

                components.previousState = components.currentState;
            }

            // Check for the 1-second delay before starting the timer
            if (components.timerPaused && components.startDelayClock.getElapsedTime().asSeconds() > 0.5f) {
                components.timerPaused = false;  // Start the timer after delay
                components.timerClock.restart(); // Restart the timer clock
            }

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

void handleLevelCompleteEvent(sf::Event& event, GameComponents& components) {
    components.menu->handleLevelCompleteEvent(
        event, components.currentState, *components.physicsManager,
        *components.boat, components.timeRemaining, components.score
    );
    components.timeRemaining = 30.0f;
    components.timerPaused = true; // Ensure timer is paused during victory screen
    components.timerClock.restart();
}



// Handle events in the Playing state
void handlePlayingStateEvent(sf::RenderWindow& window, sf::Event& event, GameComponents& components) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (components.pauseButton->isMouseOver(window)) {
            components.currentState = GameState::Paused;
            components.timerPaused = true;
        }
    }

    // Update the timer if not paused
    if (!components.timerPaused) {
        float elapsed = components.timerClock.restart().asSeconds();
        components.timeRemaining -= elapsed;
        if (components.timeRemaining <= 0.0f) {
            components.timeRemaining = 0.0f;
            components.currentState = GameState::LevelComplete; // Transition to LevelComplete
            components.menu->setLevelResult(LevelResult::Failed);
        }

        // Update the timer text
        components.timerText.setString("Time: " + std::to_string(static_cast<int>(components.timeRemaining)));
    }
}



// Handle events in the Paused state
void handlePausedStateEvent(sf::Event& event, GameComponents& components) {
    components.pauseMenu->handleEvent(event, components.currentState);
    if (components.currentState == GameState::MainMenu) {
        components.boat->respawnBoat(*components.physicsManager, components.currentLevel);
        components.secondBoat->respawnBoat(*components.physicsManager, components.currentLevel);
        components.timeRemaining = 30.0f;
        components.timerPaused = true;
        components.isReloaded = true; // Mark game as reloaded
    }
}

void handleControlsStateEvent(sf::RenderWindow& window, sf::Event& event, GameComponents& components) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (components.backButton->isMouseOver(window)) {
            components.currentState = GameState::MainMenu;
            components.boat->respawnBoat(*components.physicsManager, components.currentLevel);
            components.secondBoat->respawnBoat(*components.physicsManager, components.currentLevel);
            components.timeRemaining = 30.0f;
            components.timerPaused = true;
            components.isReloaded = true; // Mark game as reloaded
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

            // Unpause timer and restart the clock
            components.timerPaused = false;
            components.timerClock.restart();

            // Update timer text immediately
            components.timerText.setString("Time: " + std::to_string(static_cast<int>(components.timeRemaining)));
        }
    }
}
// Modify the function signature to accept the window
void handleMenuEvent(sf::RenderWindow& window, sf::Event& event, GameComponents& components) { // Correct: Takes window
    if (components.menu != nullptr) {
        components.menu->handleEvent(event, components.currentState, 
                                     *components.physicsManager, *components.boat, 
                                     *components.secondBoat, components.timeRemaining, 
                                     components.score, components.currentLevel);

        if (components.menu->hasLevelChanged()) {
            components.menu->resetLevelChangedFlag();

            if (components.currentLevel == 1) {
                components.physicsManager->reset(level_1());
                components.boat->setCurrentLevelData(level_1());
                components.boat->setFinishLineX(1000.0f);
                components.physicsManager->renderGround(window); // Correct: Uses window
            } else if (components.currentLevel == 2) {
                components.physicsManager->reset(level_2());
                components.boat->setCurrentLevelData(level_2());
                components.boat->setFinishLineX(10000.0f);
                components.physicsManager->renderGround(window); // Correct: Uses window
            }

            components.timeRemaining = 30.0f;
            components.timerPaused = true;
            components.timerClock.restart();
        }
    }
}

