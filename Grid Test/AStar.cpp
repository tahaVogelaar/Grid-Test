#include "AStar.h"
#include "game.h"

int getDistance(const sf::Vector2i& posA, const sf::Vector2i& posB)
{
	int disX = abs(posA.x - posB.x);
	int disY = abs(posA.y - posB.y);
	if (disX > disY)
		return 14 * disY + 10 * (disX - disY);
	return 14 * disX + 10 * (disY - disX);
}

void resetPath(std::queue<Tile*>& Qopen, std::queue<Tile*>& Qclosed)
{
	while (!Qopen.empty())
	{
		Qopen.front()->reset();
		Qopen.pop();
	}
	while (!Qclosed.empty())
	{
		Qclosed.front()->reset();
		Qclosed.pop();
	}
}

AStarPath AStar(Map& map, const sf::Vector2i& playerPos, const sf::Vector2i& targetCoor, std::unordered_set<int>& walkeble)
{
	std::queue<Tile*> Qopen, Qclosed;
	Tile* startTile = map.getTileByCordinate(playerPos);
	Tile* endTile = map.getTileByCordinate(targetCoor);
	if (startTile == nullptr || endTile == nullptr)
	{
		resetPath(Qopen, Qclosed);
		return AStarPath(false);
	}
	else if (startTile->getCoordinate() == endTile->getCoordinate())
	{
		AStarPath a(true);
		a.path.push_back(endTile);
		resetPath(Qopen, Qclosed);
		return a;
	}

	startTile->open = true;
	Qopen.push(startTile);

	Tile* current;
	Tile* neighbour = nullptr;

	while (!Qopen.empty())
	{
		current = Qopen.front();
		current->open = false;

		Qopen.pop();
		current->closed = true;
		Qclosed.push(current);

		if (current == endTile)
		{
			AStarPath path(true);
			Tile* parentTile = endTile;
			while (parentTile != nullptr)
			{
				path.maxCells++;
				path.path.push_back(parentTile);
				parentTile = parentTile->parent;
			}
			resetPath(Qopen, Qclosed);
			return path;
		}

		for (short i = 0; i < 4; i++)
		{
			neighbour = map.getNeighbour(current->getCoordinate(), i);
			if (neighbour != nullptr && walkeble.find(neighbour->getTileId()) != walkeble.end() && !neighbour->closed)
			{
				int dis = current->Gcost + getDistance(current->getCoordinate(), neighbour->getCoordinate());
				if (neighbour->Gcost > dis || !neighbour->open)
				{
					neighbour->Gcost = dis;
					neighbour->Hcost = getDistance(neighbour->getCoordinate(), endTile->getCoordinate());
					neighbour->Fcost = neighbour->Gcost + neighbour->Hcost;

					neighbour->parent = current;

					if (!neighbour->open)
					{
						neighbour->open = true;
						Qopen.push(neighbour);
					}
				}
			}
		}
	}
	resetPath(Qopen, Qclosed);
	return AStarPath(false);
}