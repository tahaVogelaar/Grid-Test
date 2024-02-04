#pragma once
#include "SFML/Graphics.hpp"
#include "map.h"
#include <vector>
#include <unordered_map>

class GameObject;

enum TileType {
	employe,
	customer,
	EandC,
	blocked
};

class View
{
	sf::RenderWindow& window;
	sf::Vector2f position;
	float moveSpeed = 3, shiftMoveSpeed;

public:
	View(sf::RenderWindow& w, float cellSize);
	~View();

	bool update(float deltaTime);
	void draw();

	sf::Vector2f getPosition() { return position; }
	GameObject* getCurrentObj() { return currentObj; }

	int cash = std::numeric_limits<int>::max();
private:
	sf::RectangleShape backGroundUi;
	sf::RectangleShape close;
	sf::VertexArray renderButtons;
	std::unordered_map<TileType, std::vector<GameObject>> objMap;
	sf::Text cashText;
	bool clicked = false;
	bool down;

	// ui
	const sf::Vector2f navBarSize;
	const float scale;
	const float tileScale;
	const float tileSpace;
	float downPos, upPos, buttonTop;
	TileType currentTile = TileType::EandC;

	GameObject* currentObj = nullptr;

	void loadTiles();
	void uiUp();
	void uiDown();
	void selectTileType(TileType t);
};

class GameObject {
	int order, blockId, textureId, cost;
	sf::Color color;
	TileType tileType;

public:
	GameObject() {}

	GameObject(int tId, int bId, int Cost, sf::Color c, TileType tt) : tileType(tt), textureId(tId), blockId(bId), cost(Cost), color(c) { }
	void setOrder(short o) { order = o; }
	int getBlockId() const { return blockId; }
	int getTextureId() const { return textureId; }
	int getCost() const { return cost; }
	short getOrder() const { return order; }
	sf::Color getColor() const { return color; }
	TileType getTileType() const { return tileType; }

	bool compareByCost(const GameObject& a, const GameObject& b) {
		return a.getCost() < b.getCost();
	}
};