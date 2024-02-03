#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <chrono>
#include "AStar.h"

class Player {
	sf::Vector2f position;
	sf::RectangleShape plr;
	const float& deltaTime;

	float moveSpeed = 2;

	const float reloadAStarTime = 10000;
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	// a star
	int currentPosition = 0;
	AStarPath path;

	sf::Vector2i targetPosition;
	bool arived = false;
	Map& map;

public:
	Player(Map& m, const float& deltaTime);
	sf::Vector2f getPosition();
	void setPosition(const sf::Vector2f& move);
	void moveToPosition(const sf::Vector2f& move);
	void draw(sf::RenderWindow& w, const sf::Vector2f& vpos);
	bool update();
	bool findNewPath();
	void newTargetPosition();

	bool operator==(const Player& other)
	{
		return this == &other;
	}
	bool operator!=(const Player& other)
	{
		return this != &other;
	}

	Player& operator=(const Player& other)
	{
		this->arived = other.arived;
		this->begin = other.begin;
		this->currentPosition = other.currentPosition;
		this->moveSpeed = other.moveSpeed;
		this->path.maxCells = other.path.maxCells;
		this->path.path = other.path.path;
		this->path.pathFound = other.path.pathFound;
		this->position = other.position;
		this->targetPosition = other.targetPosition;
		return *this;
	}
};