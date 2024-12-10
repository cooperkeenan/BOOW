// Timer.cpp
#include "Timer.h"

// Constructor: Initializes the timer's text and sets its initial properties
Timer::Timer(float initialTime, sf::Font& font, unsigned int charSize, sf::Color color)
    : initialTime(initialTime), remainingTime(initialTime), timerText("", font, charSize)
{
    timerText.setFillColor(color);
    timerText.setPosition(10.0f, 10.0f);  // Top-left corner with padding
    timerText.setString("Time: " + std::to_string(static_cast<int>(remainingTime)) + "s");

    // Center the text vertically within its character size
    sf::FloatRect textRect = timerText.getLocalBounds();
    timerText.setOrigin(textRect.left, textRect.top);
}

// Resets the timer to the initial time and updates the displayed text
void Timer::reset() {
    remainingTime = initialTime;
    timerText.setString("Time: " + std::to_string(static_cast<int>(remainingTime)) + "s");
}

// Updates the remaining time based on deltaTime and updates the displayed text
void Timer::update(float deltaTime) {
    if (remainingTime > 0.0f) {
        remainingTime -= deltaTime;
        if (remainingTime < 0.0f)
            remainingTime = 0.0f;

        timerText.setString("Time: " + std::to_string(static_cast<int>(remainingTime)) + "s");
    }
}

// Draws the timer text onto the window
void Timer::draw(sf::RenderWindow& window) const {
    window.draw(timerText);
}

// Returns the remaining time
float Timer::getRemainingTime() const {
    return remainingTime;
}