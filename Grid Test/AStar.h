#pragma once
#include "SFML/Graphics.hpp"
#include "map.h"
#include <queue>

struct AStarPath {
	bool pathFound = false;
	int maxCells = 0;
	std::vector<std::pair<sf::Vector2f, Cell*>> path;
	AStarPath() {}
	AStarPath(bool v) { pathFound = v; }
};

AStarPath AStar(Map& map, const sf::Vector2f& playerPos, const sf::Vector2i& targetCor);