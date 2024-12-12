#include "EventHandler.h"
#include "GameSetup.h"

// Forward declarations for internal event handlers used in handleGameEvents
void handleLevelCompleteEvent(sf::Event& event, GameComponents& components);
void handleMenuEvent(sf::Event& event, GameComponents& components);
void handlePlayingStateEvent(sf::RenderWindow& window, sf::Event& event, GameComponents& components);
void handlePausedStateEvent(sf::Event& event, GameComponents& components);
void handleControlsStateEvent(sf::RenderWindow& window, sf::Event& event, GameComponents& components);
void handleEscapeKeyEvent(sf::Event& event, GameComponents& components);


// The main function to process all events based on the current game state.
void handleGameEvents(sf::RenderWindow& window, sf::Event& event, GameComponents& components) {
    // Level Complete
    if (components.currentState == GameState::LevelComplete) {
        handleLevelCompleteEvent(event, components);
    } else {
        handleMenuEvent(event, components);
    }

    // Depending on the current game state, handle specific state events.
    switch (components.currentState) {
        case GameState::Playing:
            // initialize timers and delays.
            if (components.previousState != components.currentState) {
                if (components.previousState == GameState::MainMenu || 
                    components.previousState == GameState::LevelSelection || 
                    components.isReloaded) {
                    components.timeRemaining = 30.0f;
                    components.timerPaused = true;
                    components.startDelayClock.restart();
                    components.isReloaded = false;
                }

                components.previousState = components.currentState;
            }

            // Start the timer after a short delay, allowing the player to prepare.
            if (components.timerPaused && components.startDelayClock.getElapsedTime().asSeconds() > 0.5f) {
                components.timerPaused = false;
                components.timerClock.restart();
            }

            handlePlayingStateEvent(window, event, components);
            break;

        case GameState::Paused:
            // When paused, delegate to the pause event handler.
            handlePausedStateEvent(event, components);
            break;

        case GameState::Controls:
            // In the Controls screen, handle events that might send the player back to main menu.
            handleControlsStateEvent(window, event, components);
            break;

        default:
            // For states like MainMenu or LevelSelection, the menu handler suffices.
            break;
    }

    // Handle pressing the Escape key to pause or resume.
    handleEscapeKeyEvent(event, components);
}

// If level complete show menu 
void handleLevelCompleteEvent(sf::Event& event, GameComponents& components) {
    components.menu->handleLevelCompleteEvent(event, components.currentState, *components.physicsManager, *components.boat, components.timeRemaining, components.score);
}

// Handle menu-related events (MainMenu, LevelSelection, or Controls states).
void handleMenuEvent(sf::Event& event, GameComponents& components) {
    // Delegate menu interactions to the menu object.
    components.menu->handleEvent(
        event, 
        components.currentState,
        *components.physicsManager,
        *components.boat,
        *components.secondBoat,
        components.timeRemaining,
        components.score,
        components.currentLevel
    );
}

// Handle events in the Playing state, including clicking the pause button or timer updates.
void handlePlayingStateEvent(sf::RenderWindow& window, sf::Event& event, GameComponents& components) {
    // Check for pause button clicks.
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (components.pauseButton->isMouseOver(window)) {
            components.currentState = GameState::Paused;
            components.timerPaused = true;
        }
    }

    // If the timer isn't paused, decrement time and check if time ran out.
    if (!components.timerPaused) {
        float elapsed = components.timerClock.restart().asSeconds();
        components.timeRemaining -= elapsed;
        if (components.timeRemaining <= 0.0f) {
            components.timeRemaining = 0.0f;
            components.currentState = GameState::LevelComplete;
            components.menu->setLevelResult(LevelResult::Failed);
        }

        // Update the on-screen timer text.
        components.timerText.setString("Time: " + std::to_string(static_cast<int>(components.timeRemaining)));
    }
}

// Handle events in the Paused state, such as resuming or returning to the main menu.
void handlePausedStateEvent(sf::Event& event, GameComponents& components) {
    components.pauseMenu->handleEvent(event, components.currentState);
    if (components.currentState == GameState::MainMenu) {
        // If returning to the main menu, reset the boat and world state.
        components.boat->respawnBoat(*components.physicsManager, components.currentLevel);
        components.secondBoat->respawnBoat(*components.physicsManager, components.currentLevel);
        components.timeRemaining = 30.0f;
        components.timerPaused = true;
        components.isReloaded = true;
    }
}

// Handle events in the Controls state, such as going back to the main menu.
void handleControlsStateEvent(sf::RenderWindow& window, sf::Event& event, GameComponents& components) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (components.backButton->isMouseOver(window)) {
            components.currentState = GameState::MainMenu;
            components.boat->respawnBoat(*components.physicsManager, components.currentLevel);
            components.secondBoat->respawnBoat(*components.physicsManager, components.currentLevel);
            components.timeRemaining = 30.0f;
            components.timerPaused = true;
            components.isReloaded = true; 
        }
    }
}

// Handle Escape key presses to toggle between Playing and Paused states.
void handleEscapeKeyEvent(sf::Event& event, GameComponents& components) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        if (components.currentState == GameState::Playing) {
            // Pause the game if currently playing.
            components.currentState = GameState::Paused;
            components.timerPaused = true;
        } else if (components.currentState == GameState::Paused) {
            // Resume the game if currently paused.
            components.currentState = GameState::Playing;
            components.timerPaused = false;
            components.timerClock.restart();
            components.timerText.setString("Time: " + std::to_string(static_cast<int>(components.timeRemaining)));
        }
    }
}
