#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

// Represents a collectable item in the game world.
class Collectable {
public:
    Collectable(b2World& world, const sf::Vector2f& position, float radius); // Constructor: Creates a collectable in the physics world
    void render(sf::RenderWindow& window);                               // Renders the collectable to the window
    b2Body* getBody() const;                                               // Returns the Box2D body of the collectable
    void markCollected();                                                // Marks the collectable as collected
    bool isCollected() const;                                             // Checks if the collectable has been collected

private:
    b2Body* body;         // Box2D body for physics simulation
    sf::CircleShape shape; // Shape for rendering the collectable
    bool collected;       // Flag indicating if the collectable has been collected
};