#pragma once
#include "SFML/Graphics.hpp"
#include "map.h"
#include <unordered_set>
#include <queue>
#include <iostream>

struct AStarPath {
	bool pathFound = false;
	int maxCells = 0;
	std::vector<Tile*> path;
	AStarPath() {}
	AStarPath(bool v) { pathFound = v; }

	void draw(sf::RenderWindow& window, const float cellSize, const sf::Vector2f& pos)
	{
		if (maxCells == 0) return;
		float color = 255, addition = 255 / maxCells;
		sf::CircleShape dot(10);
		dot.setOrigin(5, 5);
		for (size_t i = maxCells - 1; i >= 1; i--)
		{
			dot.setPosition(static_cast<sf::Vector2f>(path[i]->getCoordinate()) * cellSize);
			dot.move(-pos);
			dot.move(cellSize / 2.f, cellSize / 2.f);
			dot.setFillColor(sf::Color(color, 0, 0));
			color -= addition;
			window.draw(dot);
		}

		dot.setPosition(static_cast<sf::Vector2f>(path[0]->getCoordinate()) * cellSize);
		dot.move(-pos);
		dot.move(cellSize / 2.f, cellSize / 2.f);
		dot.setFillColor(sf::Color(0, 0, 0));
		window.draw(dot);
	}
};

AStarPath AStar(Map& map, const sf::Vector2i& playerPos, const sf::Vector2i& targetCor, std::unordered_set<int>& walkeble);