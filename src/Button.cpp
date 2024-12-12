// Button.cpp
#include "Button.h"

// Constructor: Initializes the button shape and text without setting the font
Button::Button(const std::string& text, sf::Vector2f size, int charSize, sf::Color bgColor, sf::Color textColor) {
    // Initialize the button shape
    button.setSize(size);
    button.setFillColor(bgColor);

    // Initialize the button text
    buttonText.setString(text);
    buttonText.setCharacterSize(charSize);
    buttonText.setFillColor(textColor);


    // Center the text within the button (temporary position; will be updated after setting the font)
    buttonText.setOrigin(0, 0);  
    buttonText.setPosition(size.x / 2.0f, size.y / 2.0f);
}

// Sets the font for the button text and re-centers the text
void Button::setFont(sf::Font& font) {
    buttonFont = &font;
    buttonText.setFont(*buttonFont);

    // Recalculate the origin based on the new font to ensure accurate centering
    sf::FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin(textRect.left + textRect.width / 2.0f,
                         textRect.top + textRect.height / 2.0f);
    // Reposition the text to the center of the button
    sf::Vector2f buttonPos = button.getPosition();
    buttonText.setPosition(buttonPos.x + button.getSize().x / 2.0f,
                           buttonPos.y + button.getSize().y / 2.0f);
}

// Sets the position of the button on the window and repositions the text accordingly
void Button::setPosition(sf::Vector2f pos) {
    button.setPosition(pos);
    
    // Reposition the text to maintain centering
    if (buttonFont != nullptr) {  // Ensure the font has been set
        buttonText.setPosition(pos.x + button.getSize().x / 2.0f,
                               pos.y + button.getSize().y / 2.0f);
    }
}

// Draws the button and its text onto the window
void Button::draw(sf::RenderWindow& window) const {
    window.draw(button);
    window.draw(buttonText);
}

// Checks if the mouse is currently over the button
bool Button::isMouseOver(sf::RenderWindow& window) const {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);  // Get mouse position relative to the window
    sf::FloatRect btnBounds = button.getGlobalBounds();      // Get button bounds
    return btnBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}


// Retrieves the size of the button
sf::Vector2f Button::getSize() const {
    return button.getSize();
}