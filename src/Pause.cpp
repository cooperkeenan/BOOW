// Pause.cpp
#include "Pause.h"

// Constructor: Initializes the overlay and the Resume button
Pause::Pause(sf::RenderWindow& window, sf::Font& font)
    : window(window),
      resumeButton("Resume", {200, 50}, 20, sf::Color::Green, sf::Color::White)
{
    // Initialize the semi-transparent overlay
    overlay.setSize(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));  // Black with 150 alpha for transparency
    overlay.setPosition(0, 0);

    // Initialize the Resume button
    resumeButton.setFont(font);
    // Center the Resume button on the screen
    resumeButton.setPosition({
        (static_cast<float>(window.getSize().x) - resumeButton.getSize().x) / 2.0f,
        (static_cast<float>(window.getSize().y) - resumeButton.getSize().y) / 2.0f
    });
}

// Handles events when the game is paused
void Pause::handleEvent(const sf::Event& event, GameState& currentState) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (resumeButton.isMouseOver(window)) {
            currentState = GameState::Playing;  // Resume the game
        }
    }
}

// Draws the pause menu (overlay and Resume button)
void Pause::draw() {
    window.draw(overlay);
    resumeButton.draw(window);
}