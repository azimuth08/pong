#ifndef UTIL_H_
#define UTIL_H_

#include <cmath>
#include <SFML/Graphics.hpp>

namespace util {

    struct scoreboard 
    {
        int lhs, rhs;
    };
    sf::Vector2f checkBounds(sf::Shape&, sf::FloatRect);
    void moveBall(sf::CircleShape&, sf::Vector2f&, float&);
    bool checkCollision(sf::CircleShape&, sf::RectangleShape&);
    bool inRange(sf::Shape&, sf::Shape&);
    // all functions for game physics 
    void bounce(sf::CircleShape&, sf::Vector2f&, float&, sf::RectangleShape&);
    bool checkBallBounds(sf::CircleShape&, sf::Vector2f&, float& ,sf::FloatRect&, scoreboard&);

}


#endif
