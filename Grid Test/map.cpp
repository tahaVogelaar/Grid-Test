#include "map.h"
#include "game.h"
#include "noise generation.h"
#include <bitset>
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <unordered_set>

int strToInt(const std::string& s)
{
	int v = 0, power = 0;
	for (int i = s.length() - 1; i >= 0; i--)
	{
		v += (s[i] - '0') * std::pow(10, power);
		power++;
	}
	return v;
}

unsigned short strToUShort(const std::string& s)
{
	unsigned short v = 0;
	int power = 0;
	for (int i = s.length() - 1; i >= 0; i--)
	{
		v += (s[i] - '0') * std::pow(10, power);
		power++;
	}
	return v;
}

void Map::addVertex()
{
	sf::Vertex v1;
	renderMap.append(v1);
	renderMap.append(v1);
	renderMap.append(v1);
	renderMap.append(v1);
}

void Tile::reset()
{
	closed = false;
	open = false;
	parent = nullptr;
	Fcost = 0;
	Hcost = 0;
	Gcost = 0;
}

Tile::Tile(const sf::Vector2i& coor, const int& tId, const int& bId, const sf::Color& c) : coordinate(coor), tileId(bId), color(c)
{
	textureid.id = tId;
}

void Tile::setCost(const sf::Vector2i& start, const sf::Vector2i& end)
{
	Hcost = 10 * sqrt((end.x - coordinate.x) * (end.x - coordinate.x) + (end.y - coordinate.y) * (end.y - coordinate.y));
	Gcost = 10 * sqrt((start.x - coordinate.x) * (start.x - coordinate.x) + (start.y - coordinate.y) * (start.y - coordinate.y));
	Fcost = Hcost + Gcost;
}

Map::Map(sf::RenderWindow& w, int viewGridSize, std::unordered_map<TextureId, TextureVarible>& t, std::string fileName, const sf::Texture& textur) : window(w), windowSize(w.getSize()), viewGridSize(viewGridSize),
tileSize(std::max(windowSize.x, windowSize.y) / viewGridSize), renderMap(sf::Quads), textures(t), fileString(fileName), texture(textur)
{
	for (int x = 0; x < viewGridSize+1; x++)
	{
		for (int y = 0; y < viewGridSize+1; y++)
		{
			addVertex();
		}
	}
	readFile();
}

void Map::update(const sf::Vector2f& viewPos, sf::Event& event, bool canPlace, GameObject& obj)
{
	mousePosition = sf::Mouse::getPosition(window) + static_cast<sf::Vector2i>(viewPos);

	if (sf::Mouse::getPosition(window).x < windowSize.x && sf::Mouse::getPosition(window).y < windowSize.y && 0 <= sf::Mouse::getPosition(window).x && 0 <= sf::Mouse::getPosition(window).y &&
		sf::Mouse::isButtonPressed(sf::Mouse::Left) && *cashPtr >= obj.getCost() && canPlace)
	{
		sf::Vector2i coor(static_cast<int>(mousePosition.x / tileSize), static_cast<int>(mousePosition.y / tileSize));
		bool placed = placeTile(coor, obj);
		giveTileTexture(*getTileByCordinate(coor));
		if (placed) *cashPtr -= obj.getCost();
	}


	// render
	sf::Vector2f position;
	sf::Vector2f offset(std::fmod(-viewPos.x, tileSize), std::fmod(-viewPos.y, tileSize));
	sf::Vector2i coordinate = sf::Vector2i(viewPos / tileSize);
	int c = 0;
	TextureVarible tVar(textures[TextureId(true)]);

	for (int x = 0; x < viewGridSize + 1; x++)
	{
		for (int y = 0; y < viewGridSize + 1; y++)
		{
			position.x = (x * tileSize) + offset.x;
			position.y = (y * tileSize) + offset.y;
			coordinate.x = x + (viewPos.x / tileSize);
			coordinate.y = y + (viewPos.y / tileSize);

			if (getTileByCordinate(coordinate))
			{
				tVar = textures[getTileByCordinate(coordinate)->getTextureId()];
				renderMap[c].color = getTileByCordinate(coordinate)->getColor();
				renderMap[c+1].color = getTileByCordinate(coordinate)->getColor();
				renderMap[c+2].color = getTileByCordinate(coordinate)->getColor();
				renderMap[c+3].color = getTileByCordinate(coordinate)->getColor();
			}
			else
			{
				tVar = textures[TextureId(true)];
				renderMap[c].color = sf::Color::White;
				renderMap[c+1].color = sf::Color::White;
				renderMap[c+2].color = sf::Color::White;
				renderMap[c+3].color = sf::Color::White;
			}


			renderMap[c].position = position;
			renderMap[c].texCoords.x = tVar.x;
			renderMap[c].texCoords.y = tVar.y;
			c++;
			renderMap[c].position = position + sf::Vector2f(tileSize, 0);
			renderMap[c].texCoords.x = tVar.x + tVar.size;
			renderMap[c].texCoords.y = tVar.y;
			c++;
			renderMap[c].position = position + sf::Vector2f(tileSize, tileSize);
			renderMap[c].texCoords.x = tVar.x + tVar.size;
			renderMap[c].texCoords.y = tVar.y + tVar.size;
			c++;
			renderMap[c].position = position + sf::Vector2f(0, tileSize);
			renderMap[c].texCoords.x = tVar.x;
			renderMap[c].texCoords.y = tVar.y + tVar.size;
			c++;
		}
	}
}

bool Map::placeTile(const sf::Vector2i& corr, const GameObject& obj)
{
	Tile* tile = getTileByCordinate(corr);
	if (!tile)
	{
		Tile t(corr, obj.getTextureId(), obj.getTileId(), obj.getColor());
		map.insert(std::make_pair(corr, t));
		return true;
	}
	else if (tile->getTextureId().id == obj.getTextureId() && tile->getTileId() == obj.getTileId()) return false;

	tile->setTextureId(obj.getTextureId());
	tile->setTileId(obj.getTileId());
	tile->setColor(obj.getColor());

	return true;
}

void Map::giveTileBit(Tile& tile)
{
	Tile* n = nullptr;

	for (short i = 0; i < 4; i++)
	{
		n = getNeighbour(tile.getCoordinate(), i);

		if (n != nullptr)
		{
			if (n->getTextureId().id == tile.getTextureId().id && n->getTileId() == tile.getTileId())
				tile.setTextureBit(false, i);
			else
			{
				tile.setTextureBit(true, i);
			}
		}
		else
			tile.setTextureBit(true, i);
	}
}

void Map::giveTileTexture(Tile& tile)
{
	if (getNeighbour(tile.getCoordinate(), 0)) giveTileBit(*getNeighbour(tile.getCoordinate(), 0));
	if (getNeighbour(tile.getCoordinate(), 1)) giveTileBit(*getNeighbour(tile.getCoordinate(), 1));
	if (getNeighbour(tile.getCoordinate(), 2)) giveTileBit(*getNeighbour(tile.getCoordinate(), 2));
	if (getNeighbour(tile.getCoordinate(), 3)) giveTileBit(*getNeighbour(tile.getCoordinate(), 3));
	giveTileBit(tile);
}

void Map::draw()
{
	window.draw(renderMap, &texture);
}

Tile* Map::getTileByPos(const sf::Vector2f& pos)
{
	sf::Vector2i corr(static_cast<int>(pos.x / tileSize), static_cast<int>(pos.y / tileSize));
	if (map.find(corr) != map.end())
		return &map[corr];
	return nullptr;
}

Tile* Map::getNeighbour(const sf::Vector2i& coor, const short& n)
{
	switch (n)
	{
	case 0:
		return getTileByCordinate(sf::Vector2i(coor.x, coor.y - 1));
		break;
	case 1:
		return getTileByCordinate(sf::Vector2i(coor.x + 1, coor.y));
		break;
	case 2:
		return getTileByCordinate(sf::Vector2i(coor.x, coor.y + 1));
		break;
	case 3:
		return getTileByCordinate(sf::Vector2i(coor.x - 1, coor.y));
		break;
	}
	return nullptr;
}

Tile* Map::getTileByCordinate(const sf::Vector2i& coor)
{
	if (map.find(coor) != map.end())
		return &map[coor];
	return nullptr;
}

sf::Vector2i Map::getWindowSize()
{
	return windowSize;
}

sf::Vector2i Map::getRandomTile()
{
	return std::next(map.begin(), randomNumber(0, map.size() - 1))->second.getCoordinate();
}

void Map::readFile()
{
	std::ifstream inputFile;
	inputFile.open(fileString);

	std::string line, numb;
	int x, y, tId, bId;
	sf::Color c;
	if (inputFile.peek() == std::ifstream::traits_type::eof()) return;
	while (std::getline(inputFile, line))
	{
		int iteration = 0;
		for (int i = 0; i <= line.length(); i++)
		{
			if (line[i] == ',' || i == line.length())
			{
				switch (iteration)
				{
				case 0:
					x = strToInt(numb);
					break;
				case 1:
					y = strToInt(numb);
					break;
				case 2:
					tId = strToInt(numb);
					break;
				case 3:
					bId = strToInt(numb);
					break;
				case 4:
					c.r = strToInt(numb);
					break;
				case 5:
					c.g = strToInt(numb);
					break;
				case 6:
					c.b = strToInt(numb);
					break;
				}
				iteration++;
				numb.clear();
			}
			else
				numb += line[i];
		}
		GameObject obj(tId, bId, c);
		placeTile(sf::Vector2i(x, y), obj);
		giveTileTexture(*getTileByCordinate(sf::Vector2i(x, y)));
		numb.clear();
	}

	inputFile.close();
}

Map::~Map()
{
	std::cout << "saving file\n";

	int saves = 0;
	std::chrono::high_resolution_clock::time_point t = std::chrono::high_resolution_clock::now();

	std::ofstream outputFile;
	outputFile.open(fileString);

	for (auto& i : map)
	{
		outputFile << i.first.x << ',' << i.first.y << ',' << i.second.getTextureId().id << ',' << i.second.getTileId() << ',' << unsigned short(i.second.getColor().r) << ',' << unsigned short(i.second.getColor().g) << ',' << unsigned short(i.second.getColor().b) << '\n';
		saves++;
	}

	std::cout << "saving took: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t).count() << " milliseconds and saved: " << saves << " tiles\n";
}