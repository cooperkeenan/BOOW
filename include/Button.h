// Button.h
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    // Constructor: Initializes the button with text, size, character size, background color, and text color
    Button(const std::string& text, sf::Vector2f size, int charSize, sf::Color bgColor, sf::Color textColor);
    
    // Sets the font for the button text
    void setFont(sf::Font& font);
    
    // Sets the position of the button on the window
    void setPosition(sf::Vector2f pos);
    
    // Draws the button and its text onto the window
    void draw(sf::RenderWindow& window) const;
    
    // Checks if the mouse is currently over the button
    bool isMouseOver(sf::RenderWindow& window) const;
    
    // Retrieves the size of the button
    sf::Vector2f getSize() const;

    void setText(const std::string& text);


private:
    sf::RectangleShape button;  // The visual rectangle representing the button
    sf::Text buttonText;        // The text displayed on the button
    sf::Font* buttonFont = nullptr;  // Pointer to the font used by the button text
    bool hasCrossedFinishLine = false; 
};