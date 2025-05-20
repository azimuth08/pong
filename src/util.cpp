#include "../include/util.h"

namespace util {

    float getDistance(sf::Shape& s1, sf::Shape& s2) {
        return 0.0;
    }

    bool checkCollision(sf::CircleShape& ball, sf::RectangleShape& player) {
        bool ret = false;
        sf::FloatRect playerBounds = player.getGlobalBounds();

        sf::Vector2f ballLHS = {ball.getPosition().x - ball.getRadius(), ball.getPosition().y}; 
        sf::Vector2f ballRHS = {ball.getPosition().x + ball.getRadius(), ball.getPosition().y}; 
        
        // if left or right most point of ball in player, collision has been made
        if(playerBounds.contains(ballLHS) || playerBounds.contains(ballRHS) ) {
            ret = true;
        }
        
        return ret;
    }

    bool inRange(float x1, float x2, float k) {
        return k > x1 && k < x2;
    }
}



