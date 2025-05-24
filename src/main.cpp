#include <SFML/Graphics.hpp>
#include "../include/Size.h"
#include "../include/util.h"

// cpp standard libs
#include <cstdlib> // for random number generator
#include <optional>
#include <cmath>
#include <iostream>

// declare all functions
void init_game(sf::RenderWindow&, sf::RectangleShape& , sf::RectangleShape&, sf::CircleShape&, sf::Vector2f&, float&, util::scoreboard&);
void updateGame(sf::RenderWindow&, sf::RectangleShape&, sf::RectangleShape&, sf::CircleShape& ,sf::Vector2f, sf::Vector2f);
void drawMap(sf::RenderWindow&);
void verifyBounds(sf::RectangleShape& player, sf::RectangleShape& enemy, sf::FloatRect bounds);
void init_ball(sf::CircleShape&, sf::Vector2f&, float& ,sf::FloatRect);
bool round(sf::RectangleShape& , sf::RectangleShape& , sf::CircleShape& , sf::Vector2f& , 
    sf::FloatRect& , float& , float& , float& , sf::RenderWindow& , util::scoreboard& );
void displayScore(util::scoreboard&, sf::RenderWindow&);
void resetRound(sf::CircleShape&, sf::RenderWindow&, sf::Vector2f&, float&);

int main()
{
    // create main window
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({Size::windowWidth, Size::windowHeight}), "CMake SFML Project");
    window.setFramerateLimit(144);
    window.setVerticalSyncEnabled(true);

    // create entities
    sf::RectangleShape player(Size::playerSize);
    sf::RectangleShape enemy(Size::playerSize);
    sf::CircleShape ball(Size::ballSize);

    // create main boundary for entities
    sf::FloatRect windowBoundary(sf::Vector2f(0.f, 0.f), window.getDefaultView().getSize());

    // initialize velocities
    float paddleVelocity = 1000;
    float velocityMagnitude = 0;
    sf::Vector2f ballVelocity = sf::Vector2f(0,0);

    //initialize clock
    sf::Clock clock;
    
    // initialize entities to correct starting position
    util::scoreboard points;
    init_game(window, player, enemy, ball, ballVelocity, velocityMagnitude, points);

    // start program
    while (window.isOpen())
    {
        sf::Time time = clock.restart();
        float deltaTime = time.asSeconds();
        // checks for events every frame
        while (const std::optional event = window.pollEvent())
        {
            // closes window if user closes it
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        while(round(player,enemy,ball,ballVelocity,windowBoundary,velocityMagnitude,deltaTime,paddleVelocity,window,points)) 
        {   
            time = clock.restart();
            deltaTime = time.asSeconds();
            
        }
        resetRound(ball,window,ballVelocity,velocityMagnitude);
        

    }
    
}

// resets or initializes the game to its starting point
void init_game(sf::RenderWindow& window, sf::RectangleShape& player, sf::RectangleShape& enemy, sf::CircleShape& ball, sf::Vector2f& ballVelocity, 
    float& velocityMag, util::scoreboard& points)
{
    // initialize size variables and boundary
    sf::Vector2u windowSize = window.getSize();
    sf::FloatRect windowBoundary(sf::Vector2f(0.f, 0.f), window.getDefaultView().getSize());

    // reset scoreboard
    points.lhs = 0;
    points.rhs = 0;

    // generate and initialize the players to correct spots
    sf::Vector2f ballPos = { windowSize.x / 2.f , windowSize.y / 2.f };

    player.setOrigin({ (Size::playerSize.x / 2), (Size::playerSize.y / 2) });
    enemy.setOrigin({ (Size::playerSize.x / 2), (Size::playerSize.y / 2) });

    player.setPosition({ 100.f, 500.f });
    enemy.setPosition({ 1820.f, 500.f });

    // spawn ball in random area in the center
    ball.setOrigin({ Size::ballSize / 2, Size::ballSize / 2 });
    ball.setPosition(ballPos);
    init_ball(ball, ballVelocity, velocityMag ,windowBoundary);

    window.draw(player);
    window.draw(enemy);
    window.draw(ball);

    window.display();
}

// updates frame depending on the key pressed
void updateGame(sf::RenderWindow& window ,sf::RectangleShape& player, sf::RectangleShape& enemy, sf::CircleShape& ball,sf::Vector2f pPosition, sf::Vector2f ePosition,
        util::scoreboard& points)
{
    // resets frame to black background
    window.clear(sf::Color::Black);

    // create background first
    drawMap(window);

    //update the player's positions and score
    window.draw(player);
    window.draw(enemy);
    window.draw(ball);
    
    displayScore(points, window);

    // ends frame by displaying drawing
    window.display();
}

void drawMap(sf::RenderWindow& window)
{
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2f middleLineSize = { 25.f, 100.f };
    // draw middle point of the field
    int count = 0;
    int  spacing = 0;
    int spacingInterval = 190;
    while (count < 8)
    {
        sf::RectangleShape b(middleLineSize);

        sf::Vector2f originPos = { middleLineSize.x / 2, middleLineSize.y / 2 };
        sf::Vector2f pos = { (float)(windowSize.x / 2), spacing + middleLineSize.y / 2};

        b.setOrigin(originPos);
        b.setPosition(pos);

        // draw the line
        window.draw(b);

        spacing += spacingInterval;
        count++;
    }
}

void verifyBounds(sf::RectangleShape& player, sf::RectangleShape& enemy, sf::FloatRect bounds)
{
    player.setPosition(util::checkBounds(player, bounds));
    enemy.setPosition(util::checkBounds(enemy, bounds));
}



void init_ball(sf::CircleShape& ball, sf::Vector2f& ballVelocity, float& velocityMag ,sf::FloatRect bounds)
{
    // create random seed
    srand(static_cast<unsigned int>(time(nullptr)));

    // set ball to random position in y axis
    float randomYCoordinate = rand() % (int)bounds.size.y;
    ball.setPosition({ ball.getPosition().x, randomYCoordinate });

    // create random direction for ball with set Magnitude
    // We want ||m|| to be constant, but the components to be random
    // within reason
    velocityMag = 500;
    ballVelocity.y = rand() % 400;
    ballVelocity.x = -sqrt(pow(velocityMag,2) - pow(ballVelocity.y,2));


}



bool round(sf::RectangleShape& player, sf::RectangleShape& enemy, sf::CircleShape& ball, sf::Vector2f& ballVelocity, 
    sf::FloatRect& windowBoundary, float& velocityMagnitude, float& deltaTime, float& paddleVelocity, sf::RenderWindow& window, util::scoreboard& points
)
{
    bool ret = true;
    // check for player keyboard movements
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        // move up
        player.move({ 0.f,-paddleVelocity* deltaTime});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        // move down
        player.move({ 0.f, paddleVelocity * deltaTime});
    }
    
    // check collision
    if(util::checkCollision(ball, player)) {
        player.setFillColor(sf::Color::Red);
        util::bounce(ball,ballVelocity,velocityMagnitude, player);
    } else {
        player.setFillColor(sf::Color::White);
    }

    if(util::checkCollision(ball, enemy)) {
        enemy.setFillColor(sf::Color::Red);
        util::bounce(ball,ballVelocity,velocityMagnitude, enemy);

    } else {
        enemy.setFillColor(sf::Color::White);
    }

    if (util::checkBallBounds(ball,ballVelocity,velocityMagnitude,windowBoundary, points))
    {
        ret = false;
    }

    util::moveBall(ball, ballVelocity, deltaTime);
    verifyBounds(player, enemy, windowBoundary);
    updateGame(window, player, enemy, ball,player.getPosition(), enemy.getPosition(), points);
    return ret;
}

void displayScore(util::scoreboard& points, sf::RenderWindow& window) 
{
    std::string scoreS = std::to_string(points.lhs) + "   :   " + std::to_string(points.rhs) ;

    sf::Font font;
    if (!font.openFromFile(RESOURCES_PATH "PixeloidSans.ttf"))
    {
        std::cout << "error opening file" << std::endl;
    } 

    sf::Text text(font, scoreS, 100);
    text.setOrigin({text.getLocalBounds().size.x / 2.0f, text.getLocalBounds().size.y/ 2.0f});
    text.setPosition({window.getSize().x /2.0f ,100}); 
    window.draw(text);
}

void resetRound(sf::CircleShape& ball, sf::RenderWindow& window, sf::Vector2f& ballVelocity, float& velocityMag)
{
    // initialize size variables and boundary
    sf::Vector2u windowSize = window.getSize();
    sf::FloatRect windowBoundary(sf::Vector2f(0.f, 0.f), window.getDefaultView().getSize());


    // generate and initialize the ball to correct spots
    sf::Vector2f ballPos = { windowSize.x / 2.f , windowSize.y / 2.f };


    // spawn ball in random area in the center
    ball.setOrigin({ Size::ballSize / 2, Size::ballSize / 2 });
    ball.setPosition(ballPos);
    init_ball(ball, ballVelocity, velocityMag ,windowBoundary);

}