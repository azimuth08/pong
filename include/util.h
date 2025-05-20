#ifndef UTIL_H_
#define UTIL_H_

#include <cmath>
#include <SFML/Graphics.hpp>

namespace util {
    float getDistance(sf::Shape&, sf::Shape&);
    bool checkCollision(sf::CircleShape&, sf::RectangleShape&);
    bool inRange(sf::Shape&, sf::Shape&);
    void bounce();

}


#endif
