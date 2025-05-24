#include "../include/util.h"
#include "../include/Size.h"

namespace util {

    float getDistance(sf::Shape& s1, sf::Shape& s2) 
    {
        return 0.0;
    }

    bool checkCollision(sf::CircleShape& ball, sf::RectangleShape& player) 
    {
        bool ret = false;
        sf::FloatRect playerBounds = player.getGlobalBounds();

        sf::Vector2f ballLHS = {ball.getPosition().x - ball.getRadius(), ball.getPosition().y}; 
        sf::Vector2f ballRHS = {ball.getPosition().x + ball.getRadius(), ball.getPosition().y}; 
        
        // if left or right most point of ball in player, collision has been made
        if(playerBounds.contains(ballLHS) || playerBounds.contains(ballRHS) ) 
        {
            ret = true;
        }
        
        return ret;
    }

    bool inRange(float x1, float x2, float k) {
        return k > x1 && k < x2;
    }

    void bounce(sf::CircleShape& ball, sf::Vector2f& ballVelocity, float& velocityMag,sf::RectangleShape& paddle) 
    {

        float distanceFromCenter = ball.getPosition().y  + paddle.getPosition().y; // positive is above and neg is below
        
        if(velocityMag > 0) 
        {
            velocityMag += 30;
        } 
        else
        {
            velocityMag -= 30;
        }

        velocityMag = -velocityMag;
        // if ball hits center, flip x velocity 180
        if(distanceFromCenter == 0) 
        {

            ballVelocity.x = velocityMag * cos(M_PI);
            ballVelocity.y = velocityMag * sin(M_PI);

        }
        // if ball hits above center go 45 degrees up
        else if(distanceFromCenter > 0) 
        {
            ballVelocity.x = velocityMag * cos(M_PI/4);
            ballVelocity.y = velocityMag * -sin(M_PI/4);
        }
        // if ball hits below center go 45 degrees down
        else if(distanceFromCenter < 0) 
        {
            ballVelocity.x = velocityMag * cos(M_PI/4);
            ballVelocity.y = velocityMag * sin(M_PI/4);
        }
    }

    bool checkBallBounds(sf::CircleShape& ball, sf::Vector2f& ballVelocity, float& velocityMag, sf::FloatRect& bounds, scoreboard& points) 
    {
        bool ret = false;
        sf::Vector2f position = ball.getPosition();

        // gets the correct bounds of top and bottom of window
        float topLimit = bounds.position.y + (Size::ballSize);
        float bottomLimit = bounds.size.y - (Size::ballSize);

        if (position.y < topLimit || position.y > bottomLimit)
        {
            ballVelocity.y = -ballVelocity.y;
        }

        // checks bounds for left and right of window
        float rightLimit = bounds.size.x + (Size::ballSize);
        if (position.x > rightLimit)
        {
            ret = true;
            points.rhs ++;
        }
        float leftLimit = bounds.position.x - (Size::ballSize);
        if (position.x < leftLimit)
        {
            ret = true;
            points.lhs ++;
        } 
        
        return ret;
    }

}



