#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
#include "map.h"

class GameObject;

class View
{
	sf::Vector2f position;
	float moveSpeed = 3, shiftMoveSpeed;
	sf::RenderWindow& window;

	// dragging
	GameObject* currentObj = nullptr;

public:
	View(sf::RenderWindow& w, float cellSize);
	~View();

	sf::Vector2f getPosition() { return position; }
	bool update(float deltaTime);
	void draw();

	GameObject* getCurrentObj() { return currentObj; }

	void loadTiles();

	unsigned long long int cash = std::numeric_limits<unsigned long long int>::max();
private:
	sf::RectangleShape backGroundUi;
	bool clicked = false;
	float downPos, upPos;

	sf::RectangleShape close;

	const float scale;

	std::vector<GameObject> gameObjects;
	sf::Text cashText;
	bool down;
};

class GameObject {
	int blockId = 0, textureId = 0;
	int cost = 0;
	sf::Color color;

public:
	sf::RectangleShape shape;
	GameObject(int tId, int bId, int cost, sf::Color color)
	{
		this->textureId = tId;
		this->blockId = bId;
		this->cost = cost;
		this->color = color;
		shape.setSize(sf::Vector2f(50, 50));
		shape.setFillColor(sf::Color::Black);
	}
	int getBlockId() { return blockId; }
	int getTextureId() { return textureId; }
	int getCost() { return cost; }
	sf::Color getColor() { return color; }
};