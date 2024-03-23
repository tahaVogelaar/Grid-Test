#pragma once
#include "view.h"
#include "texture.h"
#include "SFML/Graphics.hpp"
#include <unordered_map>
#include <string>


class GameObject;

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

class Tile {
	int tileId;
	TextureId textureid;
	const sf::Vector2i coordinate;
	sf::Color color = sf::Color::White;

public:
	void setCost(const sf::Vector2i& start, const sf::Vector2i& end);
	void setTextureBit(bool b, const short& i) { textureid.bitset[i] = b; }
	//void setObject(const GameObject& obj) { textureid = obj.getTextureId(); tileId = obj.getTileId(); color = obj.getColor(); }
	void setTileId(const int& id) { this->tileId = id; }
	void setColor(const sf::Color& c) { color = c; }
	void setTextureId(const int& tid) { textureid.id = tid; }

	const int getTileId() const { return tileId; }
	const sf::Color getColor() const { return color; }
	const TextureId getTextureId() const { return textureid; }
	const sf::Vector2i getCoordinate() const { return coordinate; }

	Tile(const sf::Vector2i& cor, const int& tId, const int& bId, const sf::Color& c);
	Tile() {}

	void reset();

	// a star
	int Gcost = 0, Hcost = 0, Fcost = 0;
	bool open = false, closed = false;
	Tile* parent = nullptr;
};

class Map {
	// memory
	std::unordered_map<sf::Vector2i, Tile> map;
	std::unordered_map<TextureId, TextureVarible>& textures;

	// window and cells
	sf::RenderWindow& window;
	const sf::Vector2i windowSize;
	const float tileSize;
	sf::Vector2i mousePosition;

	// rendering
	int viewGridSize;
	sf::VertexArray renderMap;
	std::unordered_map<sf::Vector2i, TextureVarible> renderTile;
	const sf::Texture texture;

	std::string fileString;
public:
	Map(sf::RenderWindow& w, int viewGridSize, std::unordered_map<TextureId, TextureVarible>&, std::string, const sf::Texture&);
	void update(const sf::Vector2f& viewPos, sf::Event& event, bool canPlace, GameObject& obj);
	void draw();

	Tile* getTileByPos(const sf::Vector2f& pos);
	Tile* getNeighbour(const sf::Vector2i& pos, const short& n);
	Tile* getTileByCordinate(const sf::Vector2i& cor);
	sf::VertexArray getRenderMap() { return renderMap; }
	float getTileSize() { return tileSize; }
	sf::Vector2i getWindowSize();
	sf::Vector2i getRandomTile();

	bool tileExists(const Tile* tile);
	bool tileExistsCoor(const sf::Vector2i& coor);
	bool tileExistsPos(const sf::Vector2f& pos);
	int* cashPtr = nullptr; // i was lazy
	void readFile();

	~Map();

private:
	void addVertex();
	void giveTileTexture(Tile& tile);
	void giveTileBit(Tile& tile);
	bool placeTile(const sf::Vector2i& coor, const GameObject& obj);
};
