#include "Pause.h"

// Constructor: Initializes the pause menu overlay and buttons.
Pause::Pause(sf::RenderWindow& window, sf::Font& font)
    : window(window),
      resumeButton("Resume", {200, 50}, 20, sf::Color::Green, sf::Color::Black),
      mainMenuButton("Main Menu", {200, 50}, 20, sf::Color::Red, sf::Color::Black) {

    // Create a semi-transparent overlay to darken the game view.
    overlay.setSize(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y))); // Set overlay size to window size.
    overlay.setFillColor(sf::Color(0, 0, 0, 150)); // Set overlay color to semi-transparent black.
    overlay.setPosition(0, 0); // Set overlay position to top-left corner of the window.

    // Set font and position for the Resume button.
    resumeButton.setFont(font);
    resumeButton.setPosition({
        (static_cast<float>(window.getSize().x) - resumeButton.getSize().x) / 2.0f, // Center horizontally.
        (static_cast<float>(window.getSize().y) - resumeButton.getSize().y) / 2.0f - 60 // Center vertically, offset upwards.
    });

    // Set font and position for the Main Menu button.
    mainMenuButton.setFont(font);
    mainMenuButton.setPosition({
        (static_cast<float>(window.getSize().x) - mainMenuButton.getSize().x) / 2.0f, // Center horizontally.
        (static_cast<float>(window.getSize().y) - mainMenuButton.getSize().y) / 2.0f + 10 // Center vertically, offset downwards.
    });
}

// Handles events when the game is paused, specifically mouse clicks on the buttons.
void Pause::handleEvent(const sf::Event& event, GameState& currentState) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (resumeButton.isMouseOver(window)) {
            currentState = GameState::Playing; // Set the game state to Playing to resume.
        } else if (mainMenuButton.isMouseOver(window)) {
            currentState = GameState::MainMenu; // Set the game state to MainMenu to return to the main menu.
        }
    }
}

// Draws the pause menu elements (overlay and buttons) to the window.
void Pause::draw() {
    window.draw(overlay); // Draw the overlay.
    resumeButton.draw(window); // Draw the Resume button.
    mainMenuButton.draw(window); // Draw the Main Menu button.
}