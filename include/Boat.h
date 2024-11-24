#ifndef BOAT_H
#define BOAT_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class Boat {
public:
    Boat(b2World& world, const sf::Vector2f& position, const sf::Vector2f& size);
    void update();
    void render(sf::RenderWindow& window);
    void move(const b2Vec2& force);
    void move(float directionX, float directionY, float magnitude);

private:
    sf::RectangleShape boatSprite;
    b2Body* boatBody;
};

#endif // BOAT_H
