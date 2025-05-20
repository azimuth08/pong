#include <SFML/Graphics.hpp>
#include "../include/Size.h"
#include "../include/util.h"

// cpp standard libs
#include <cstdlib> // for random number generator
#include <optional>
#include <cmath>

// declare all functions
void init_game(sf::RenderWindow&, sf::RectangleShape&, sf::RectangleShape&, sf::CircleShape&, sf::Vector2f&);
void updateGame(sf::RenderWindow&, sf::RectangleShape&, sf::RectangleShape&, sf::CircleShape& ,sf::Vector2f, sf::Vector2f);
void drawMap(sf::RenderWindow&);
void verifyBounds(sf::RectangleShape& player, sf::RectangleShape& enemy, sf::CircleShape& ball, sf::FloatRect bounds);
sf::Vector2f checkBounds(sf::Shape&, sf::FloatRect);
void init_ball(sf::CircleShape&, sf::Vector2f&, sf::FloatRect);
void moveBall(sf::CircleShape&, sf::Vector2f&);

int main()
{
    // create main window
    auto window = sf::RenderWindow(sf::VideoMode({Size::windowWidth, Size::windowHeight}), "CMake SFML Project");
    window.setFramerateLimit(144);
    window.setVerticalSyncEnabled(true);

    // create entities
    sf::RectangleShape player(Size::playerSize);
    sf::RectangleShape enemy(Size::playerSize);
    sf::CircleShape ball(Size::ballSize);

    // create main boundary for entities
    sf::FloatRect windowBoundary(sf::Vector2f(0.f, 0.f), window.getDefaultView().getSize());

    // initialize velocity of ball of zero speed 
    sf::Vector2f ballVelocity = sf::Vector2f(0,0);

    // initialize entities to correct starting position
    init_game(window, player, enemy, ball, ballVelocity);

    // start program
    while (window.isOpen())
    {
        // checks for events every frame
        while (const std::optional event = window.pollEvent())
        {
            // closes window if user closes it
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // check for player keyboard movements
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            // move up
            player.move({ 0.f,-10.f });
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            // move down
            player.move({ 0.f,10.f });
        }
        moveBall(ball, ballVelocity);
        
        // check collision
        if(util::checkCollision(ball, player)) {
            player.setFillColor(sf::Color::Red);
        } else {
            player.setFillColor(sf::Color::White);
        }

        if(util::checkCollision(ball, enemy)) {
            enemy.setFillColor(sf::Color::Red);
        } else {
            enemy.setFillColor(sf::Color::White);
        }

        verifyBounds(player, enemy, ball, windowBoundary);
        updateGame(window, player, enemy, ball,player.getPosition(), enemy.getPosition());

    }
    
}

// resets or initializes the game to its starting point
void init_game(sf::RenderWindow& window, sf::RectangleShape& player, sf::RectangleShape& enemy, sf::CircleShape& ball, sf::Vector2f& ballVelocity)
{
    // initialize size variables and boundary
    sf::Vector2u windowSize = window.getSize();
    sf::FloatRect windowBoundary(sf::Vector2f(0.f, 0.f), window.getDefaultView().getSize());


    // generate and initialize the players to correct spots
    sf::Vector2f ballPos = { windowSize.x / 2.f , windowSize.y / 2.f };

    player.setOrigin({ (Size::playerSize.x / 2), (Size::playerSize.y / 2) });
    enemy.setOrigin({ (Size::playerSize.x / 2), (Size::playerSize.y / 2) });

    player.setPosition({ 100.f, 500.f });
    enemy.setPosition({ 1820.f, 500.f });

    // spawn ball in random area in the center
    ball.setOrigin({ Size::ballSize / 2, Size::ballSize / 2 });
    ball.setPosition(ballPos);
    init_ball(ball, ballVelocity, windowBoundary);

    window.draw(player);
    window.draw(enemy);
    window.draw(ball);

    window.display();
}

// updates frame depending on the key pressed
void updateGame(sf::RenderWindow& window ,sf::RectangleShape& player, sf::RectangleShape& enemy, sf::CircleShape& ball,sf::Vector2f pPosition, sf::Vector2f ePosition)
{
    // resets frame to black background
    window.clear(sf::Color::Black);

    // create background first
    drawMap(window);

    //update the player's positions
    window.draw(player);
    window.draw(enemy);
    window.draw(ball);

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

void verifyBounds(sf::RectangleShape& player, sf::RectangleShape& enemy, sf::CircleShape& ball, sf::FloatRect bounds)
{
    player.setPosition(checkBounds(player, bounds));
    enemy.setPosition(checkBounds(enemy, bounds));
}

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

void init_ball(sf::CircleShape& ball, sf::Vector2f& ballVelocity, sf::FloatRect bounds)
{
    // create random seed
    srand(static_cast<unsigned int>(time(nullptr)));

    // set ball to random position in y axis
    float randomYCoordinate = rand() % (int)bounds.size.y;
    ball.setPosition({ ball.getPosition().x, randomYCoordinate });

    // create random direction for ball with set Magnitude
    // We want ||m|| to be constant, but the components to be random
    // within reason
    float m = 2.0;
    ballVelocity.y = (rand() % 13) / 10;
    ballVelocity.x = pow(m,2) - pow(ballVelocity.y,2);


}

void moveBall(sf::CircleShape& ball, sf::Vector2f& ballVelocity){
    ball.setPosition({ ball.getPosition().x + ballVelocity.x, ball.getPosition().y + ballVelocity.y });
}

