#include <SFML/Graphics.hpp>
#include "Boat.h"
#include "PhysicsManager.h"
#include "GameState.h"
#include "Menu.h"
#include "Pause.h"
#include "Constants.h"
#include "AIController.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Boat Out of Water");

    sf::Font font;
    if (!font.loadFromFile("../../arial.ttf")) {  // Adjust this path if needed
        std::cerr << "Failed to load font" << std::endl;
        return -1;
    }

    // Initialize menu and game components
    Menu menu(window, font);
    PhysicsManager physicsManager;
    Boat playerBoat(physicsManager.getWorld(), physicsManager, sf::Vector2f(150.0f, 100.0f), sf::Vector2f(40.0f, 20.0f));
    Boat secondBoat(physicsManager.getWorld(), physicsManager, sf::Vector2f(150.0f, 200.0f), sf::Vector2f(40.0f, 20.0f), true);

    // Define AI input sequence
    std::vector<AIInput> aiInputs = {
        {sf::Keyboard::Enter, 5.0f}, // Idle for 4 seconds
        {sf::Keyboard::Up, 2.0f},
        {sf::Keyboard::Right, 1.5f},
        {sf::Keyboard::Down, 2.0f},
        {sf::Keyboard::Left, 1.0f}
    };
    AIController aiController(secondBoat, aiInputs);

    sf::Clock clock;
    sf::Clock gravityClock; // Added clock specifically for gravity tracking
    bool playerGravityApplied = false;
    bool secondBoatGravityApplied = false;
    float lerpFactor = 2.0f;
    GameState currentState = GameState::MainMenu;
    GameState previousState = currentState;

    // Timer setup
    sf::Clock timerClock; // Clock to track elapsed time
    float timeRemaining = 30.0f; // Timer starts at 30 seconds
    bool timerPaused = false; // Track if the timer is paused
    sf::Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(20);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(10.0f, 10.0f);

    // Define a separate view for the game
    sf::View gameView(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

    // Initialize Pause and Resume Buttons
    Button pauseButton("Pause", {100, 40}, 20, sf::Color::Yellow, sf::Color::Black);
    pauseButton.setFont(font);
    pauseButton.setPosition({WINDOW_WIDTH - 120, 20});

    // Initialize the Pause class
    Pause pauseMenu(window, font);

    // Controls screen setup
    std::vector<std::string> controlsText = {
        "Use left and right arrow keys to rotate",
        "Use up and down arrow keys to control speed",
        "Use escape to pause"
    };
    Button backButton("Back", {200, 50}, 20, sf::Color::Blue, sf::Color::White);
    backButton.setFont(font);
    backButton.setPosition({300, 400});

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle menu events, possibly switching states
            menu.handleEvent(event, currentState);

            // Handle Pause and Resume button clicks
            if (currentState == GameState::Playing) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (pauseButton.isMouseOver(window)) {
                        currentState = GameState::Paused;
                        timerPaused = true; // Pause the timer
                    }
                }
            } else if (currentState == GameState::Paused) {
                pauseMenu.handleEvent(event, currentState);
                if (currentState == GameState::MainMenu) {
                    playerBoat.respawnBoat(); // Reset the player boat position
                    secondBoat.respawnBoat(); // Reset the second boat position
                    timeRemaining = 30.0f; // Reset the timer
                    timerPaused = true; // Pause the timer
                }
            } else if (currentState == GameState::Controls) {
                // Handle Back button on the Controls screen
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (backButton.isMouseOver(window)) {
                        currentState = GameState::MainMenu;
                        playerBoat.respawnBoat(); // Reset the player boat position
                        secondBoat.respawnBoat(); // Reset the second boat position
                        timeRemaining = 30.0f; // Reset the timer
                        timerPaused = true; // Pause the timer
                    }
                }
            }

            // Handle Escape Key for Pausing and Resuming
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                if (currentState == GameState::Playing) {
                    currentState = GameState::Paused;
                    timerPaused = true; // Pause the timer
                } else if (currentState == GameState::Paused) {
                    currentState = GameState::Playing;
                    timerPaused = false; // Resume the timer
                }
            }
        }

        // If we just switched to playing, initialize the camera properly
        if (previousState != currentState && currentState == GameState::Playing) {
            b2Vec2 boatPos = playerBoat.getBoatBody()->GetPosition();
            sf::Vector2f initialCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
            gameView.setCenter(initialCenter);

            playerGravityApplied = false;
            secondBoatGravityApplied = false;
            clock.restart();
            gravityClock.restart(); // Reset the gravity clock on state change

            // Ensure the timer starts correctly when transitioning from other states
            if (previousState != GameState::Paused) {
                timeRemaining = 30.0f; // Reset to 30 seconds
                timerClock.restart(); // Restart the clock for proper countdown
                timerPaused = false;  // Resume the timer
            }

            // Update timer immediately
            timerText.setString("Time: " + std::to_string(static_cast<int>(timeRemaining)));
        }

        previousState = currentState;

        // Clear the window using the default view
        window.setView(window.getDefaultView());
        window.clear(sf::Color::Black);

        if (currentState == GameState::MainMenu || currentState == GameState::LevelSelection) {
            window.setView(window.getDefaultView());
            menu.draw(currentState);
        } else if (currentState == GameState::Playing) {
            // Update timer if not paused
            if (!timerPaused) {
                timeRemaining -= timerClock.restart().asSeconds();
                if (timeRemaining < 0.0f) timeRemaining = 0.0f; // Clamp timer to 0
            }

            timerText.setString("Time: " + std::to_string(static_cast<int>(timeRemaining)));

            // Player input and boat movement
            float directionX = 0.0f; // Movement along x-axis
            float torque = 0.0f;     // Rotation

            // Process player input
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) directionX = 0.4f;    // Reduced force for forward
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) directionX = -0.4f; // Reduced force for backward
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) torque = 1.0f;      // Reduced torque for left rotation
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) torque = -1.0f;    // Reduced torque for right rotation

            // Apply forces based on player input
            playerBoat.move(directionX, 0.0f, 5.0f); // Apply movement force
            playerBoat.rotate(torque);              // Apply rotational torque

            // Apply gravity based on elapsed time
            float elapsedTime = gravityClock.getElapsedTime().asSeconds();
            physicsManager.applyGravityIfNeeded(playerGravityApplied, elapsedTime, 0.5f);

            // Check for respawn
            if (playerBoat.checkRespawnNeeded()) {
                playerBoat.respawnBoat();

                // Respawn second boat slightly below the player boat
                b2Vec2 playerPosition = playerBoat.getBoatBody()->GetPosition();
                secondBoat.getBoatBody()->SetTransform(b2Vec2(playerPosition.x, playerPosition.y + 2.0f), 0.0f);

                // Reset AIController's state
                aiController.reset();

                // Update the camera to center on the player's new position
                b2Vec2 boatPos = playerBoat.getBoatBody()->GetPosition();
                sf::Vector2f instantCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
                gameView.setCenter(instantCenter);
                window.setView(gameView);

                // Reset game variables on respawn
                clock.restart();
                gravityClock.restart(); // Restart gravity clock after respawn
                timeRemaining = 30.0f;
                timerClock.restart();
            } else {
                // Smooth camera follow logic
                b2Vec2 boatPos = playerBoat.getBoatBody()->GetPosition();
                sf::Vector2f targetCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, gameView.getCenter().y);
                sf::Vector2f currentCenter = gameView.getCenter();
                sf::Vector2f newCenter = currentCenter + lerpFactor * (targetCenter - currentCenter);
                gameView.setCenter(newCenter);
                window.setView(gameView);
            }

            // Update AI controller
            aiController.update(clock.restart().asSeconds());

            // Step physics
            physicsManager.step();
            playerBoat.update();
            secondBoat.update(); // Update second boat position

            // Render boats and track
            physicsManager.renderGround(window);
            playerBoat.render(window);
            secondBoat.render(window); // Render second boat

            window.setView(window.getDefaultView());
            window.draw(timerText);
            pauseButton.draw(window);
        } else if (currentState == GameState::Paused) {
            window.setView(gameView);
            physicsManager.renderGround(window);
            playerBoat.render(window);
            secondBoat.render(window); // Render second boat in paused state
            window.setView(window.getDefaultView());
            pauseMenu.draw();
        } else if (currentState == GameState::Controls) {
            // Draw controls instructions
            float yOffset = 100.0f;
            for (const auto& line : controlsText) {
                sf::Text text(line, font, 20);
                text.setFillColor(sf::Color::White);
                text.setPosition(50, yOffset);
                yOffset += 50;
                window.draw(text);
            }

            // Draw Back button
            backButton.draw(window);
        }

        window.display();
    }

    return 0;
}
