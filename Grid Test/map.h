#pragma once
#include "view.h"
#include "texture.h"
#include "SFML/Graphics.hpp"
#include <unordered_map>
#include <string>

class GameObject;

enum CellType {
	empty = 0,
	blocked = 1
};

namespace std {
	template <> struct hash<sf::Vector2i>
	{
		size_t operator()(const sf::Vector2i& vec) const
		{
			std::hash<int> hasher;
			auto hashx = hasher(vec.x);
			auto hashy = hasher(vec.y);

			hashx ^= hashy + 0x9e3779b9 + (hashx << 6) + (hashx >> 2);
			return hashx;
		}
	};
}

class Cell {
	int blockId;
	TextureId textureid;
	const sf::Vector2i cordinate;
	sf::Color color;

public:
	void setCost(const sf::Vector2i& start, const sf::Vector2i& end);
	void setBlockId(const int& id) { this->blockId = id; }
	void setColor(const sf::Color& c) { color = c; }
	void setTextureId(const int& tid) { textureid.id = tid; }
	void setTextureBit(bool b, const short& i) { textureid.bitset[i] = b; }

	const int getBlockId() const { return blockId; }
	const sf::Color getColor() const { return color; }
	const TextureId getTextureId() const { return textureid; }
	const sf::Vector2i getCordinate() const { return cordinate; }

	Cell(const sf::Vector2i& cor, const int& tId, const int& bId, const sf::Color& c);
	Cell() {}

	void reset();

	// a star
	int Gcost = 0, Hcost = 0, Fcost = 0;
	bool open = false, closed = false;
	CellType cellType = CellType::empty;
	Cell* parent = nullptr;
};

class Map {
	// memory
	std::unordered_map<sf::Vector2i, Cell> map;
	std::unordered_map<TextureId, TextureVarible>& textures;

	// window and cells
	sf::RenderWindow& window;
	const float cellSize;
	const sf::Vector2i windowSize;
	sf::Vector2i mousePosition;

	// rendering
	int viewGridSize;
	sf::VertexArray renderMap;
	std::unordered_map<sf::Vector2i, TextureVarible> renderCells;

	std::string fileString;
public:
	Map(sf::RenderWindow& w, int viewGridSize, std::unordered_map<TextureId, TextureVarible>&, std::string);
	void update(const sf::Vector2f& viewPos, sf::Event& event, bool canPlace, GameObject& obj);
	void draw();

	Cell* getCellByPos(const sf::Vector2f& pos);
	Cell* getNeighbour(const sf::Vector2i& pos, const short& n);
	Cell* getCellByCordinate(const sf::Vector2i& cor);
	sf::VertexArray getRenderMap() { return renderMap; }
	float getCellSize() { return cellSize; }
	sf::Vector2i getWindowSize();
	sf::Vector2i getRandomCell();

	bool cellExists(const Cell* cell);
	bool cellExistsCor(const sf::Vector2i& cor);
	bool cellExistsPos(const sf::Vector2f& pos);
	unsigned long long int* cashPtr = nullptr; // i was lazy
	void placeCell(Cell* cor, const int& textureId, const int& blockId, const sf::Color& c);
	void readFile();

	~Map();

private:
	void addVertex();
	void giveCellTexture(Cell& cell);
	void createCell(const sf::Vector2i& cor, const int& textureId, const int& blockId, const sf::Color& c);
};