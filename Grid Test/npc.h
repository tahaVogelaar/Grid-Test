#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <chrono>
#include "AStar.h"

enum ProductType {
	Apple,
	Pear,
	Lemon
};

class Product {
	unsigned int count;
	ProductType type;
public:
	Product() {}
	Product(ProductType type) : type(type) {}
	unsigned int getCount() { return count; }

	template<typename T>
	void operator+=(const T& value) { count += value; }
	template<typename T>
	void operator-=(const T& value) { count -= value; }
	void operator++() { count++; }
	void operator--() { count--; }
};

class Npc {
	bool moveToPosition(sf::Vector2f pos);
	bool findNewPath();
	void newTargetPosition();

public:
	Npc(Map& m, const float& deltaTime);
	void setPosition(const sf::Vector2f& move);
	void draw(sf::RenderWindow& w, const sf::Vector2f& vpos);
	bool update();

	sf::Vector2f getPosition();
	AStarPath getPath() { return path; }

	bool operator==(const Npc& other)
	{
		return this == &other;
	}
	bool operator!=(const Npc& other)
	{
		return this != &other;
	}

	Npc& operator=(const Npc& other)
	{
		this->begin = other.begin;
		this->currentPath = other.currentPath;
		this->moveSpeed = other.moveSpeed;
		this->path.maxCells = other.path.maxCells;
		this->path.path = other.path.path;
		this->path.pathFound = other.path.pathFound;
		this->position = other.position;
		return *this;
	}

private:
	sf::Vector2f position;
	sf::RectangleShape player;
	const float& deltaTime;
	Map& map;

	float moveSpeed = .5;

	const float reloadAStarTime = 10000;
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	// a star
	int currentPath = 0;
	AStarPath path;
	std::unordered_set<int> walkeble = { 1 };

	// shopping
	std::list<Product> list;
	std::list<Product> inventory;
	sf::Vector2i target;
};