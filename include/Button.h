#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    Button(const std::string& text, sf::Vector2f size, int charSize, sf::Color bgColor, sf::Color textColor); // Constructor: Initializes the button
    void setFont(sf::Font& font); // Sets the font for the button text
    void setPosition(sf::Vector2f pos); // Sets the position of the button
    void draw(sf::RenderWindow& window) const; // Draws the button onto the window
    bool isMouseOver(sf::RenderWindow& window) const; // Checks if the mouse is over the button
    sf::Vector2f getSize() const; // Retrieves the size of the button

private:
    sf::RectangleShape button; // The visual rectangle representing the button
    sf::Text buttonText;     // The text displayed on the button
    sf::Font* buttonFont = nullptr; // Pointer to the font used by the button text
    bool hasCrossedFinishLine = false; //Unnecessary variable remove this
};