#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
#include <unordered_map>

#include "game.h"
#include "map.h"

namespace sf {
	class TextBox;
}
class GameObject;

enum TileType {
	employee = 0,
	customer = 1,
	EandC = 2,
	blocked = 3
};

class View
{
	sf::RenderWindow& window;
	sf::Vector2f position;
	float moveSpeed = 3, shiftMoveSpeed;
	const sf::Texture texture;

public:
	View(sf::RenderWindow& w, float cellSize, const sf::Texture);
	~View();

	bool update(float deltaTime);
	void updateUi(sf::Event& event);
	void draw();

	sf::Vector2f getPosition() { return position; }
	GameObject* getCurrentObj() { return currentObj; }

	int cash = std::numeric_limits<int>::max();
private:
	sf::RectangleShape backGroundUi;
	sf::RectangleShape close; // navigation bar buttons
	sf::VertexArray renderButtons;
	std::unordered_map<TileType, sf::RectangleShape> navButtons;
	std::unordered_map<TileType, std::vector<GameObject>> objMap;
	sf::Text cashText;
	bool clicked = false;
	bool down;

	// ui
	const float scale;
	const sf::Vector2f navBarSize;
	const float navSpace;
	const float tileScale, tileSpace;
	float downPos, upPos, buttonTop, tileY;

	// other
	TileType currentTile = TileType::EandC;
	GameObject* currentObj = nullptr;

	void loadTiles();
	void uiUp();
	void uiDown();
	void selectTileType();

	// keys
	bool pressedTab = false, pressedLeftArraw = false, pressedRightArrow = false;
};

namespace sf {
	class TextBox {
		sf::Text text;
		bool lineDir = false;

	public:
		TextBox() { text.setFont(font); text.setCharacterSize(24); }
		TextBox(const sf::String& string, const sf::Vector2f& position, const float& size, const short& strSize)
		{
			text.setString(string);
			text.setCharacterSize(strSize);
			text.setPosition(position);
			text.setFont(font);

			float scaleFactor = std::min(size / text.getLocalBounds().width, size / text.getLocalBounds().height); text.setScale(scaleFactor, scaleFactor);
		}

		void setPosition(const sf::Vector2f& position) { text.setPosition(position); }
		void setSetCharSize(const int& size) { text.setCharacterSize(size); }
		void setSize(const float& size) { float scaleFactor = std::min(size / text.getLocalBounds().width, size / text.getLocalBounds().height); text.setScale(scaleFactor, scaleFactor); }
		void setString(const std::string& string) { text.setString(string); }
		void setLineDir(bool b) {
			lineDir = b; if (b) { sf::FloatRect textBounds = text.getLocalBounds(); text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f); }
		}
		sf::Text* getText() { return &text; }
	};
}

class GameObject {
	int tileId, textureId, cost;
	sf::Color color;
	TileType tileType;
	sf::TextBox text;

public:
	GameObject() {}

	GameObject(int& tId, int& tiId, const sf::Color& c) { textureId = tId; tileId = tiId; color = c; }

	GameObject(int tId, int bId, int Cost, sf::Color c, TileType tt) : tileType(tt), textureId(tId), tileId(bId), cost(Cost), color(c) { text.setString(std::to_string(cost)); }
	int getTileId() const { return tileId; }
	int getTextureId() const { return textureId; }
	int getCost() const { return cost; }
	sf::Color getColor() const { return color; }
	TileType getTileType() const { return tileType; }
	void setPosition(const sf::Vector2f& position) { text.setPosition(position); }

	bool compareByCost(const GameObject& a, const GameObject& b) {
		return a.getCost() < b.getCost();
	}

	bool operator<(const GameObject& other) const
	{
		return cost < other.cost;
	}
};