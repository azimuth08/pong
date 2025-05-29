#ifndef UTIL_H_
#define UTIL_H_

#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>
namespace util {

    struct scoreboard 
    {
        int lhs, rhs;
    };

    enum class CollisionType 
    {
        None,
        Middle,
        Top, 
        Bottom,

    };

    sf::Vector2f checkBounds(sf::Shape&, sf::FloatRect);
    void moveBall(sf::CircleShape&, sf::Vector2f&, float&);
    bool checkCollision(sf::CircleShape&, sf::RectangleShape&);
    bool contains(sf::RectangleShape&, sf::Vector2f&);
    bool inRange(sf::Shape&, sf::Shape&);
    // all functions for game physics 
    void bounce(sf::CircleShape&, sf::Vector2f&, sf::RectangleShape&);
    bool checkBallBounds(sf::CircleShape&, sf::Vector2f& ,sf::FloatRect&, scoreboard&);

}


#endif
