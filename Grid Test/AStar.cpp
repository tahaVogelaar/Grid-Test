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

void resetPath(std::queue<Cell*>& Qopen, std::queue<Cell*>& Qclosed)
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

AStarPath AStar(Map& map, const sf::Vector2f& playerPos, const sf::Vector2i& targetCor)
{
	std::queue<Cell*> Qopen, Qclosed;
	Cell* startCell = map.getCellByPos(playerPos);
	Cell* endCell = map.getCellByCordinate(targetCor);
	if (startCell == nullptr || endCell == nullptr)
	{
		resetPath(Qopen, Qclosed);
		return AStarPath(false);
	}
	else if (startCell->getCordinate() == endCell->getCordinate())
	{
		AStarPath a(true);
		a.path.push_back(std::pair<sf::Vector2f, Cell*>(static_cast<sf::Vector2f>(endCell->getCordinate()) * map.getCellSize(), endCell));
		resetPath(Qopen, Qclosed);
		return a;
	}

	startCell->open = true;
	Qopen.push(startCell);

	Cell* current;
	Cell* neighbour = nullptr;

	while (!Qopen.empty())
	{
		current = Qopen.front();
		current->open = false;

		Qopen.pop();
		current->closed = true;
		Qclosed.push(current);

		if (current == endCell)
		{
			AStarPath path(true);
			Cell* parentCell = endCell->parent;
			path.path.push_back(std::pair<sf::Vector2f, Cell*>(static_cast<sf::Vector2f>(endCell->getCordinate()) * map.getCellSize(), parentCell));
			while (parentCell->parent != nullptr)
			{
				path.maxCells++;
				path.path.push_back(std::pair<sf::Vector2f, Cell*>(static_cast<sf::Vector2f>(endCell->getCordinate()) * map.getCellSize(), parentCell));
				parentCell = parentCell->parent;
			}
			resetPath(Qopen, Qclosed);
			return path;
		}

		for (short i = 0; i < 4; i++)
		{
			neighbour = map.getNeighbour(current->getCordinate(), i);
			if (neighbour != nullptr /* && neighbour->cellType != CellType::blocked */ && !neighbour->closed)
			{
				int dis = current->Gcost + getDistance(current->getCordinate(), neighbour->getCordinate());
				if (neighbour->Gcost > dis || !neighbour->open)
				{
					neighbour->Gcost = dis;
					neighbour->Hcost = getDistance(neighbour->getCordinate(), endCell->getCordinate());
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