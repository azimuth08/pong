#ifndef UTIL_H_
#define UTIL_H_

#include <cmath>
#include <SFML/Graphics.hpp>

namespace util {

    struct scoreboard 
    {
        int lhs, rhs;
    };

    float getDistance(sf::Shape&, sf::Shape&);
    bool checkCollision(sf::CircleShape&, sf::RectangleShape&);
    bool inRange(sf::Shape&, sf::Shape&);
    // all functions for game physics 
    void bounce(sf::CircleShape&, sf::Vector2f&, float&, sf::RectangleShape&);
    bool checkBallBounds(sf::CircleShape&, sf::Vector2f&, float& ,sf::FloatRect&, scoreboard&);

}


#endif
