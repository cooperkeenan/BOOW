// Timer.h
#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Timer {
public:
    // Constructor: Initializes the timer with a starting time, font, character size, and color
    Timer(float initialTime, sf::Font& font, unsigned int charSize = 24, sf::Color color = sf::Color::White);

    // Resets the timer to the initial time
    void reset();

    // Updates the timer based on the elapsed time (deltaTime)
    void update(float deltaTime);

    // Draws the timer on the provided window
    void draw(sf::RenderWindow& window) const;

    // Retrieves the remaining time
    float getRemainingTime() const;

private:
    float initialTime;          // The starting time for the countdown
    float remainingTime;        // The current remaining time

    sf::Text timerText;         // The SFML Text object to display the timer
};