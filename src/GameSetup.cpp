#include "GameSetup.h"
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "PhysicsManager.h"
#include "Boat.h"
#include "Pause.h"
#include "Button.h"
#include "Constants.h"
#include "AIController.h" // Include for AIController
#include <vector>
#include <iostream>
#include <string>

GameComponents initializeGame(sf::RenderWindow& window) {
    GameComponents components;

    // Load Font
    if (!components.font.loadFromFile("../img/RobotoMono-Regular.ttf")) {
        std::cerr << "Failed to load font\n";
        std::exit(-1); // Exit if font fails to load
    }

    // Initialize menu and game components
    components.menu = new Menu(window, components.font);
    components.physicsManager = new PhysicsManager();
    components.boat = new Boat(components.physicsManager->getWorld(), *components.physicsManager, sf::Vector2f(150.0f, 100.0f), sf::Vector2f(40.0f, 20.0f));

    // Initialize second boat for AI
    components.secondBoat = new Boat(components.physicsManager->getWorld(), *components.physicsManager, sf::Vector2f(150.0f, 200.0f), sf::Vector2f(40.0f, 20.0f), true);

    // Define AI input sequence
    components.aiInputs = {
        {sf::Keyboard::Enter, 2.0f},
        {sf::Keyboard::Up, 2.0f},
        {sf::Keyboard::Right, 1.5f},
        {sf::Keyboard::Down, 2.0f},
        {sf::Keyboard::Left, 1.0f}
    };

    components.aiController = new AIController(*components.secondBoat, components.aiInputs);



    components.clock.restart();
    components.gravityApplied = false;
    components.secondBoatGravityApplied = false;
    components.lerpFactor = 2.0f; // Smoothness factor for camera movement
    components.currentState = GameState::MainMenu;
    components.previousState = components.currentState;

    // Timer setup
    components.timeRemaining = 30.0f;
    components.timerPaused = false;
    components.timerClock.restart();
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

    // Define a separate view for the game
    components.gameView = sf::View(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

    // Initialize Pause and Resume Buttons
    components.pauseButton = new Button("Pause", {100, 40}, 20, sf::Color::Yellow, sf::Color::Black);
    components.pauseButton->setFont(components.font);
    components.pauseButton->setPosition({WINDOW_WIDTH - 120, 20});

    // Initialize the Pause class
    components.pauseMenu = new Pause(window, components.font);

    // Controls screen setup
    components.controlsText = {
        "Use left and right arrow keys to rotate",
        "Use up and down arrow keys to control speed",
        "Use escape to pause"
    };
    components.backButton = new Button("Back", {200, 50}, 20, sf::Color::Blue, sf::Color::White);
    components.backButton->setFont(components.font);
    components.backButton->setPosition({300, 400});

    return components;
}

void handlePlayingState(GameComponents& components) {
    if (components.previousState != components.currentState && components.currentState == GameState::Playing) {
        b2Vec2 boatPos = components.boat->getBoatBody()->GetPosition();
        sf::Vector2f initialCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
        components.gameView.setCenter(initialCenter);

        components.gravityApplied = false;
        components.secondBoatGravityApplied = false;
        components.clock.restart();

        if (components.previousState != GameState::Paused) {
            components.timeRemaining = 30.0f;
            components.timerClock.restart();
            components.timerPaused = false;
        }

        components.timerText.setString("Time: " + std::to_string(static_cast<int>(components.timeRemaining)));
    }

    components.previousState = components.currentState;
}
