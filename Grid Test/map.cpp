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

void Cell::reset()
{
	closed = false;
	open = false;
	parent = nullptr;
	Fcost = 0;
	Hcost = 0;
	Gcost = 0;
}

Cell::Cell(const sf::Vector2i& cor, const int& tId, const int& bId, const sf::Color& c) : cordinate(cor), blockId(bId), color(c)
{
	textureid.id = tId;
}

void Cell::setCost(const sf::Vector2i& start, const sf::Vector2i& end)
{
	Hcost = 10 * sqrt((end.x - cordinate.x) * (end.x - cordinate.x) + (end.y - cordinate.y) * (end.y - cordinate.y));
	Gcost = 10 * sqrt((start.x - cordinate.x) * (start.x - cordinate.x) + (start.y - cordinate.y) * (start.y - cordinate.y));
	Fcost = Hcost + Gcost;
}

Map::Map(sf::RenderWindow& w, int viewGridSize, std::unordered_map<TextureId, TextureVarible>& t, std::string fileName) : window(w), windowSize(w.getSize()), viewGridSize(viewGridSize),
cellSize(std::max(windowSize.x, windowSize.y) / viewGridSize), renderMap(sf::Quads), textures(t), fileString(fileName)
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

	if (canPlace && 0 < mousePosition.x && mousePosition.x < windowSize.x &&
		0 < mousePosition.y && sf::Mouse::getPosition(window).y < windowSize.y && sf::Mouse::isButtonPressed(sf::Mouse::Left) && *cashPtr >= obj.getCost())
	{
		Cell* cell = getCellByPos(static_cast<sf::Vector2f>(mousePosition));
		if (cell)
		{
			if (cell->getTextureId().id != obj.getTextureId() || cell->getTextureId().id == obj.getTextureId() && cell->getBlockId() != obj.getBlockId())
			{
				placeCell(getCellByPos(static_cast<sf::Vector2f>(mousePosition)), obj.getTextureId(), obj.getBlockId(), obj.getColor());
				*cashPtr -= obj.getCost();
			}
		}
		else
		{
			*cashPtr -= obj.getCost();
			sf::Vector2i corr(static_cast<int>(mousePosition.x / cellSize), static_cast<int>(mousePosition.y / cellSize));
			createCell(corr, obj.getTextureId(), obj.getBlockId(), obj.getColor());
		}
	}

	sf::Vector2f position;
	sf::Vector2f offset(std::fmod(-viewPos.x, cellSize), std::fmod(-viewPos.y, cellSize));
	sf::Vector2i cordinate;
	int c = 0;

	for (int x = 0; x < viewGridSize + 1; x++)
	{
		for (int y = 0; y < viewGridSize + 1; y++)
		{
			position.x = (x * cellSize) + offset.x;
			position.y = (y * cellSize) + offset.y;
			cordinate.x = x + (viewPos.x / cellSize);
			cordinate.y = y + (viewPos.y / cellSize);

			if (getCellByCordinate(cordinate))
			{
				tVar = textures[getCellByCordinate(cordinate)->getTextureId()];
			}

			renderMap[c].position = position;
			renderMap[c].texCoords.x = tVar.x;
			renderMap[c].texCoords.y = tVar.y;
			c++;
			renderMap[c].position = position + sf::Vector2f(cellSize, 0);
			renderMap[c].texCoords.x = tVar.x + tVar.size;
			renderMap[c].texCoords.y = tVar.y;
			c++;
			renderMap[c].position = position + sf::Vector2f(cellSize, cellSize);
			renderMap[c].texCoords.x = tVar.x + tVar.size;
			renderMap[c].texCoords.y = tVar.y + tVar.size;
			c++;
			renderMap[c].position = position + sf::Vector2f(0, cellSize);
			renderMap[c].texCoords.x = tVar.x;
			renderMap[c].texCoords.y = tVar.y + tVar.size;
			c++;
		}
	}
}

void Map::placeCell(Cell* cell, const int& textureId, const int& blockId, const sf::Color& c)
{
	cell->setTextureId(textureId);
	cell->setBlockId(blockId);
	cell->setColor(c);

	giveCellTexture(*cell);
	for (short i = 0; i < 4; i++)
		if (getNeighbour(cell->getCordinate(), i))
			giveCellTexture(*getNeighbour(cell->getCordinate(), i));
}

void Map::createCell(const sf::Vector2i& cor, const int& tId, const int& bId, const sf::Color& c)
{
	Cell cell(cor, tId, bId, c);
	map.insert(std::make_pair(cor, cell));

	giveCellTexture(cell);
	for (short i = 0; i < 4; i++)
		if (getNeighbour(cell.getCordinate(), i))
			giveCellTexture(*getNeighbour(cell.getCordinate(), i));
}

void Map::giveCellTexture(Cell& cell)
{
	for (size_t i = 0; i < 4; i++)
	{
		if (getNeighbour(cell.getCordinate(), i))
			cell.setTextureBit(!(getNeighbour(cell.getCordinate(), i)->getTextureId().id == cell.getTextureId().id && getNeighbour(cell.getCordinate(), i)->getBlockId() == cell.getBlockId()), i);
		else
			cell.setTextureBit(true, i);
	}
}
void Map::draw()
{
	window.draw(renderMap, &texture);
}

Cell* Map::getCellByPos(const sf::Vector2f& pos)
{
	sf::Vector2i corr(static_cast<int>(pos.x / cellSize), static_cast<int>(pos.y / cellSize));
	if (map.find(corr) != map.end())
		return &map[corr];
	return nullptr;
}

Cell* Map::getNeighbour(const sf::Vector2i& pos, const short& n)
{
	switch (n)
	{
	case 0:
		return getCellByCordinate(sf::Vector2i(pos.x, pos.y - 1));
		break;
	case 1:
		return getCellByCordinate(sf::Vector2i(pos.x + 1, pos.y));
		break;
	case 2:
		return getCellByCordinate(sf::Vector2i(pos.x, pos.y + 1));
		break;
	case 3:
		return getCellByCordinate(sf::Vector2i(pos.x - 1, pos.y));
		break;
	}
	return nullptr;
}

Cell* Map::getCellByCordinate(const sf::Vector2i& cor)
{
	if (map.find(cor) != map.end())
		return &map[cor];
	return nullptr;
}

sf::Vector2i Map::getWindowSize()
{
	return windowSize;
}

sf::Vector2i Map::getRandomCell()
{
	return std::next(map.begin(), randomNumber(0, map.size()))->second.getCordinate();
}

bool Map::cellExists(const Cell* cell)
{
	return map.find(cell->getCordinate()) != map.end() && cell != nullptr;
}

void Map::readFile()
{
	std::ifstream inputFile;
	inputFile.open(fileString);

	std::string line, numb;
	int x, y, tId, bId;
	sf::Color c;
	Cell* currentCell = &map[sf::Vector2i(0, 0)];

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
		createCell(sf::Vector2i(x, y), tId, bId, c);
		giveCellTexture(*currentCell);
		for (size_t i = 0; i < 4; i++)
			if (getNeighbour(currentCell->getCordinate(), i))
				giveCellTexture(*getNeighbour(currentCell->getCordinate(), i));
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
		outputFile << i.first.x << ',' << i.first.y << ',' << i.second.getTextureId().id << ',' << i.second.getBlockId() << ',' << unsigned short(i.second.getColor().r) << ',' << unsigned short(i.second.getColor().g) << ',' << unsigned short(i.second.getColor().b) << '\n';
		saves++;
	}

	std::cout << "saving took: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t).count() << " milliseconds and saved: " << saves << " tiles\n";
}