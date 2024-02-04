#include "player.h"
#include "noise generation.h"
#include "game.h"

Player::Player(Map& m, const float& deltaTime) : deltaTime(deltaTime), map(m)
{
    plr.setSize(sf::Vector2f(25, 25));
    plr.setOrigin(plr.getSize().x / 2, plr.getSize().y / 2);

    position = sf::Vector2f(map.getRandomCell().x, map.getRandomCell().y);
    targetPosition = map.getRandomCell();
    position = sf::Vector2f(targetPosition.x * map.getCellSize(), targetPosition.y * map.getCellSize());
    findNewPath();
}

sf::Vector2f Player::getPosition() { return position; }

void Player::setPosition(const sf::Vector2f& move)
{
    position = move;
}

void Player::moveToPosition(const sf::Vector2f& pos)
{
    sf::Vector2f direction = pos - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0.0f && distance > moveSpeed * deltaTime)
    {
        position += (direction / distance) * moveSpeed * deltaTime;
    }
    else if (distance < moveSpeed * deltaTime)
    {
        position = pos;
    }
}

bool Player::findNewPath()
{
    return true;
}

void Player::newTargetPosition()
{
    targetPosition = map.getRandomCell();
}

bool Player::update()
{
    return true;
}

void Player::draw(sf::RenderWindow& w, const sf::Vector2f& vpos)
{
    plr.setPosition(position - vpos);
    if(plr.getPosition().x - plr.getSize().x >= 0 &&
        plr.getPosition().y - plr.getSize().y >= 0 &&
        plr.getPosition().x <= 2000 &&
        plr.getPosition().y <= 2000)
        w.draw(plr);
}