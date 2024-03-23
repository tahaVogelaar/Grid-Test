#include "npc.h"
#include "noise generation.h"
#include "game.h"

Npc::Npc(Map& m, const float& deltaTime) : deltaTime(deltaTime), map(m)
{
    player.setSize(sf::Vector2f(25, 25));
    player.setOrigin(player.getSize().x / 2, player.getSize().y / 2);

    position = sf::Vector2f(5, 5);
    newTargetPosition();
    findNewPath();
}


bool Npc::update()
{
    if (currentPath == 0) // if the player is at the end of the path
    {
        newTargetPosition();
        findNewPath();
    }
    else if (walkeble.find(path.path[currentPath]->getTileId()) == walkeble.end()) // path blocked
    {
        if (!findNewPath())
        {
            newTargetPosition();
            findNewPath();
        }
    }
    else // go to the next path
    {
        if (moveToPosition((sf::Vector2f)path.path[currentPath]->getCoordinate() * map.getTileSize()))
        {
            currentPath--;
        }
    }
    return true;
}

bool Npc::findNewPath()
{
    path = AStar(map, map.getTileByPos(position)->getCoordinate(), target, walkeble);
    currentPath = path.maxCells - 1;
    if (path.pathFound)
        return true;
    return false;
}

void Npc::newTargetPosition()
{
    do
        target = map.getRandomTile();
    while (walkeble.find(map.getTileByCordinate(target)->getTileId()) == walkeble.end());
}


sf::Vector2f Npc::getPosition() { return position; }

void Npc::setPosition(const sf::Vector2f& move)
{
    position = move;
}

bool Npc::moveToPosition(sf::Vector2f pos)
{
    pos.x += (map.getTileSize() * .5f);
    pos.y += (map.getTileSize() * .5f);
    sf::Vector2f direction = pos - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0.0f && distance > moveSpeed * deltaTime)
    {
        position += (direction / distance) * moveSpeed * deltaTime;
        return false;
    }
    else if (distance < moveSpeed * deltaTime) // player arrived
    {
        position = pos;
        return true;
    }
}

void Npc::draw(sf::RenderWindow& window, const sf::Vector2f& vpos)
{
    player.setPosition(position - vpos);
    //if (player.getPosition().x + vpos.x > 0 &&
    //    player.getPosition().x + vpos.x < window.getSize().x &&
    //    player.getPosition().y + vpos.y > 0 &&
    //    player.getPosition().y + vpos.y < window.getSize().y)
        window.draw(player);
}

