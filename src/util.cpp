#include "../include/util.h"
#include "../include/Size.h"

namespace util {


    sf::Vector2f checkBounds(sf::Shape& entity, sf::FloatRect bounds)
    {
        sf::Vector2f position = entity.getPosition();

        // gets the correct bounds of top and bottom of window
        float topLimit = bounds.position.y + (Size::playerSize.y / 2);
        if (position.y < topLimit)
        {
            position.y = topLimit;
        }
        float bottomLimit = bounds.size.y - (Size::playerSize.y / 2);
        if (position.y > bottomLimit)
        {
            position.y = bottomLimit;
        }

        // checks bounds for left and right of window
        float rightLimit = bounds.size.x + (Size::playerSize.y / 2);
        if (position.x > rightLimit)
        {
            position.x = rightLimit;
        }
        float leftLimit = bounds.position.x - (Size::playerSize.x / 2);
        if (position.x < leftLimit)
        {
            position.x = leftLimit;
        }
        //position.x = std::min(position.x + (Size::playerSize.x / 2), bounds.size.x);
        //position.x = std::max(position.x - (Size::playerSize.x / 2), bounds.position.x);

        return position;
    }

    void moveBall(sf::CircleShape& ball, sf::Vector2f& ballVelocity, float& deltaTime)
    {
        ball.setPosition({ ball.getPosition().x + (ballVelocity.x * deltaTime), ball.getPosition().y + (ballVelocity.y * deltaTime)});
    }   

    bool checkCollision(sf::CircleShape& ball, sf::RectangleShape& player) 
    {
        bool ret = false;
        sf::FloatRect playerBounds = player.getGlobalBounds();

        sf::Vector2f ballLHS = {ball.getPosition().x - ball.getRadius(), ball.getPosition().y}; 
        sf::Vector2f ballRHS = {ball.getPosition().x + ball.getRadius(), ball.getPosition().y}; 
        
        // if left or right most point of ball in player, collision has been made
        if(contains(player, ballLHS) || contains(player, ballRHS))
        {
            ret = true;
        }

        //if( playerBounds.contains(ballRHS)|| playerBounds.contains(ballRHS) ) 
        //{
        //    ret = true;
        //}
        
        return ret;
    }

    bool contains(sf::RectangleShape& container, sf::Vector2f& point)
    {
        bool ret = false;
        float width = container.getLocalBounds().size.x;
        float height = container.getLocalBounds().size.y;

        float x1 = container.getGlobalBounds().position.x;
        float x2 = x1 + width;

        float y1 = container.getGlobalBounds().position.y;
        float y2 = y1 + height;

        if ( (point.x >= x1 && point.x <= x2 ) &&  (point.y >= y1 && point.y <= y2 ) )
        {
            ret = true;
        }
        else if ( (point.x >= x2 && point.x <= x1 ) &&  (point.y >= y1 && point.y <= y2 ) )
        {
            ret = true;
        }

        return ret;
    }

    bool inRange(float x1, float x2, float k) {
        return k > x1 && k < x2;
    }



    void bounce(sf::CircleShape& ball, sf::Vector2f& ballVelocity,sf::RectangleShape& paddle) 
    {

        float distanceFromCenter = paddle.getPosition().y - ball.getPosition().y  ; // positive is above and neg is below
        std::cout << distanceFromCenter << std::endl;
        CollisionType collision;
        float middleDistance = Size::ballSize * 2;
        if (distanceFromCenter < middleDistance && distanceFromCenter > -middleDistance)
        {
            collision = CollisionType::Middle;
        }
        else if (distanceFromCenter >= middleDistance) 
        {
            collision = CollisionType::Top;
        }
        else
        {
            collision = CollisionType::Bottom;
        }

        // if ball hits center, flip x velocity 180
        // if ball hits above center go x degree up based on distance from center and vice versa

        ballVelocity.x = -ballVelocity.x;

        if(collision == CollisionType::Top && ballVelocity.y == 0) 
        {
            ballVelocity.y = -abs(ballVelocity.x) ;
        }
        else if(collision == CollisionType::Top && ballVelocity.y > 0 ) 
        {
            ballVelocity.y = -ballVelocity.y;
        }
        
        if(collision == CollisionType::Bottom && ballVelocity.y == 0)
        {
            ballVelocity.y = abs(ballVelocity.x);
        }
        else if(collision == CollisionType::Bottom && ballVelocity.y < 0)
        {
            ballVelocity.y = -ballVelocity.y;
        }
    }

    bool checkBallBounds(sf::CircleShape& ball, sf::Vector2f& ballVelocity, sf::FloatRect& bounds, scoreboard& points) 
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
            points.lhs ++;
        }
        float leftLimit = bounds.position.x - (Size::ballSize);
        if (position.x < leftLimit)
        {
            ret = true;
            points.rhs ++;
        } 
        
        return ret;
    }

}



