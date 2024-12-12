#include "GameSetup.h"
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "PhysicsManager.h"
#include "Boat.h"
#include "Pause.h"
#include "Button.h"
#include "Constants.h"
#include "AIController.h" 
#include <vector>
#include <iostream>
#include <string>


int currentLevel = 1; // Track the current level being played

// Initialize all game components, setting up menus, boats, physics, and UI elements.
GameComponents initializeGame(sf::RenderWindow& window) {
    GameComponents components;

    bool isReloaded = false; // Flag to track if the game has been reloaded or reset

    // Load the font used for UI text
    if (!components.font.loadFromFile("../img/RobotoMono-Regular.ttf")) {
        std::cerr << "Failed to load font\n";
        std::exit(-1); // Exit if the font could not be loaded
    }

    // Initialize the menu system
    components.menu = new Menu(window, components.font);

    // Initialize the physics manager and main player boat
    components.physicsManager = new PhysicsManager();
    components.boat = new Boat(components.physicsManager->getWorld(), *components.physicsManager, sf::Vector2f(150.0f, 100.0f), sf::Vector2f(40.0f, 20.0f));

    // Initialize a second boat controlled by AI
    components.secondBoat = new Boat(components.physicsManager->getWorld(), *components.physicsManager, sf::Vector2f(150.0f, 200.0f), sf::Vector2f(40.0f, 20.0f), true);

    // Define a sequence of AI inputs for the second boat
    components.aiInputs = {
        {sf::Keyboard::Enter, 2.0f}, 
        {sf::Keyboard::Up, 2.0f},
        {sf::Keyboard::Right, 1.5f},
        {sf::Keyboard::Down, 2.0f},
        {sf::Keyboard::Left, 1.0f}
    };

    // Create the AI controller for the second boat
    components.aiController = new AIController(*components.secondBoat, components.aiInputs);

    // Initialize various state-related variables
    components.clock.restart();
    components.gravityApplied = false;
    components.secondBoatGravityApplied = false;
    components.lerpFactor = 2.0f; // Factor for smooth camera movement
    components.currentState = GameState::MainMenu; // Start in the main menu
    components.previousState = components.currentState;

    // Timer setup for gameplay
    components.timeRemaining = 30.0f; // Start with 30 seconds
    components.timerPaused = false;
    components.timerClock.restart(); // Clock used to decrement time
    components.timerText.setFont(components.font);
    components.timerText.setCharacterSize(20);
    components.timerText.setFillColor(sf::Color::White);
    components.timerText.setPosition(10.0f, 10.0f);

    // Score setup
    components.score = 0;
    components.scoreText.setFont(components.font);
    components.scoreText.setCharacterSize(24);
    components.scoreText.setFillColor(sf::Color::White);
    components.scoreText.setPosition(10.0f, 40.0f);

    // Define a separate view for the game (camera)
    components.gameView = sf::View(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

    // Initialize Pause button and menu
    components.pauseButton = new Button("Pause", {100, 40}, 20, sf::Color::Yellow, sf::Color::Black);
    components.pauseButton->setFont(components.font);
    components.pauseButton->setPosition({WINDOW_WIDTH - 120, 20});

    components.pauseMenu = new Pause(window, components.font);

    // Controls screen instructions
    components.controlsText = {
        "Use left and right arrow keys to rotate",
        "Use up and down arrow keys to control speed",
        "Use escape to pause"
    };

    // Back button for the controls screen
    components.backButton = new Button("Back", {200, 50}, 20, sf::Color::Blue, sf::Color::White);
    components.backButton->setFont(components.font);
    components.backButton->setPosition({300, 400});

    return components;
}

// Handle transitions and setup when entering the Playing state
void handlePlayingState(GameComponents& components) {
    // If we have just changed to the Playing state from another state
    if (components.previousState != components.currentState && components.currentState == GameState::Playing) {
        // Center the view on the boat's position for a good starting perspective
        b2Vec2 boatPos = components.boat->getBoatBody()->GetPosition();
        sf::Vector2f initialCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
        components.gameView.setCenter(initialCenter);

        // Reset gravity flags and timing
        components.gravityApplied = false;
        components.secondBoatGravityApplied = false;
        components.clock.restart();

        // If we came from a state other than paused, reset the timer and unpause it
        if (components.previousState != GameState::Paused) {
            components.timeRemaining = 30.0f;
            components.timerClock.restart();
            components.timerPaused = false;
        }

        // Update the timer text with the new time
        components.timerText.setString("Time: " + std::to_string(static_cast<int>(components.timeRemaining)));
    }

    // Record the current state as the previous state for future comparisons
    components.previousState = components.currentState;
}
