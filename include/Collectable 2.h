#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class Collectable {
public:
    Collectable(b2World& world, const sf::Vector2f& position, float radius);
    void render(sf::RenderWindow& window);
    b2Body* getBody() const;
    void markCollected();
    bool isCollected() const;

private:
    b2Body* body;
    sf::CircleShape shape;
    bool collected;
};