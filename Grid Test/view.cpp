#include <iostream>
#include <algorithm>
#include <unordered_set>

#include "view.h"
#include "texture.h"
#include "game.h"

View::View(sf::RenderWindow& w, float cellSize, const sf::Texture t) : window(w), scale(std::max(window.getSize().x, window.getSize().y) / 100), down(true), renderButtons(sf::PrimitiveType::Quads),
navBarSize(sf::Vector2f(5 * scale, 2.5f * scale)), tileScale(navBarSize.x * 2), tileSpace(tileScale / 8), navSpace(navBarSize.x / 4), texture(t)
{
	backGroundUi.setSize(sf::Vector2f(window.getSize().x, window.getSize().y * BUiRatio));
	close.setSize(navBarSize);
	close.setFillColor(sf::Color::Red);

	downPos = window.getSize().y - close.getSize().y;
	upPos = window.getSize().y - backGroundUi.getSize().y;
	buttonTop = upPos - (scale / 3);
	tileY = (upPos + navBarSize.y) + tileScale / 2;

	backGroundUi.setPosition(0, downPos);
	close.setPosition(0, downPos);
	loadTiles();

	cashText.setFont(font);
	cashText.setCharacterSize(2 * scale);
	cashText.setFillColor(sf::Color::Black);

	cashText.setString(std::to_string(cash));
	cashText.setPosition(window.getSize().x - cashText.getLocalBounds().width - 15, backGroundUi.getPosition().y);
}

void View::loadTiles()
{
	GameObject b1(1, 0, 50, sf::Color::White, TileType::customer);
	objMap[b1.getTileType()].push_back(b1);

	GameObject b2(1, 1, 50, sf::Color::Blue, TileType::EandC);
	objMap[b2.getTileType()].push_back(b2);

	GameObject b3(1, 2, 50, sf::Color::Red, TileType::blocked);
	objMap[b3.getTileType()].push_back(b3);

	GameObject b4(1, 3, 50, sf::Color::Yellow, TileType::employee);
	objMap[b4.getTileType()].push_back(b4);

	GameObject b5(1, 4, 50, sf::Color::Green, TileType::employee);
	objMap[b5.getTileType()].push_back(b5);

	std::vector<GameObject*> v;
	GameObject* heigest = nullptr;

	for (auto& i : objMap)
	{
		std::sort(i.second.begin(), i.second.end(), [](GameObject a, GameObject b)
			{
				return a < b;
			});
		sf::Vertex v;
		renderButtons.append(v); renderButtons.append(v); renderButtons.append(v); renderButtons.append(v);
	}
	currentObj = &objMap[TileType::employee][0];

	for (short i = 0; i < sizeof(TileType); i++)
	{
		sf::RectangleShape shape(navBarSize);
		shape.setFillColor(sf::Color::Blue);
		navButtons[TileType(i)] = shape;
	}
	uiDown();
}

void View::updateUi(sf::Event& event)
{
	const sf::Vector2f mousePos(sf::Mouse::getPosition(window));

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab && !pressedTab)
	{
		if (down)
			uiUp();
		else
			uiDown();
		pressedTab = true;
	}

	if (down && mousePos.y > downPos) // ui up
		uiUp();
	if (!down && mousePos.x >= close.getPosition().x && // ui down
		mousePos.x <= close.getPosition().x + close.getSize().x &&
		mousePos.y >= close.getPosition().y &&
		mousePos.y <= close.getPosition().y + close.getSize().y)
		uiDown();

	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Tab)
		pressedTab = false;

	for (int i = 0; i < sizeof(TileType); i++) // navigation bar delection
		if (!down && sf::Mouse::isButtonPressed(sf::Mouse::Left) && mousePos.x >= navButtons[TileType(i)].getPosition().x &&
			mousePos.x <= navButtons[TileType(i)].getPosition().x + close.getSize().x &&
			mousePos.y >= navButtons[TileType(i)].getPosition().y &&
			mousePos.y <= navButtons[TileType(i)].getPosition().y + close.getSize().y)
		{
			currentTile = TileType(i);
			selectTileType();
		}

	if (!down && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		for (int i = 0; i < objMap[currentTile].size() * 4; i += 4)
		{
			if (renderButtons[i].position == sf::Vector2f()) break;

			if (mousePos.x >= renderButtons[i].position.x &&
				mousePos.x <= renderButtons[i + 1].position.x &&
				mousePos.y >= renderButtons[i].position.y &&
				mousePos.y <= renderButtons[i + 3].position.y)
			{
				currentObj = &objMap[currentTile][i / 4];
			}
		}
	}
}

bool View::update(float deltaTime)
{
	const sf::Vector2f mousePos(sf::Mouse::getPosition(window));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		position.y -= moveSpeed * deltaTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		position.x -= moveSpeed * deltaTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		position.y += moveSpeed * deltaTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		position.x += moveSpeed * deltaTime;

	if (position.x < 0)
		position.x = 0;
	else if (position.x > 30000)
		position.x = 30000;
	if (position.y < 0)
		position.y = 0;
	else if (position.y > 30000)
		position.y = 30000;

	if (!down && backGroundUi.getPosition().y < mousePos.y && mousePos.y < window.getSize().y || down)
	{
		return false;
	}
	return true;
}

void View::selectTileType() // make the textures for the selected stuff or what ever am too tired to think and do something productif or simple, my brain is basicly disabled because of it. i may need some sleep and stop wriding this trash, my mom wants me to come to her and do something together but am just wriding this instad for no reason. i guess am just bored and sould leave my pc and go spend some time with my mom, but on the other end i just want to write this stupid thing. am also wriding without looking at my keyboard and it makes me feel cool, maybe thats why i dont want to leave my pc. i really gotta go tho. good night to who ever is gonna read this. i may just extend this. - 00:30 04/02/2024
{
	int c = 0;
	TextureId tId(true);
	for (size_t i = 0; i < renderButtons.getVertexCount(); i++)
	{
		renderButtons[i].position = sf::Vector2f();
	}

	for (int i = 0; i < objMap[currentTile].size(); i++)
	{
		tId.id = objMap[currentTile][i].getTextureId();

		renderButtons[c].position = sf::Vector2f(tileSpace + tileSpace * i + tileScale * i, tileY);
		renderButtons[c].color = objMap[currentTile][i].getColor();
		renderButtons[c].texCoords = sf::Vector2f(textures[tId].x, textures[tId].y);
		c++;
		renderButtons[c].position = sf::Vector2f(tileSpace + tileSpace * i + tileScale * i, tileY) + sf::Vector2f(tileScale, 0);
		renderButtons[c].color = objMap[currentTile][i].getColor();
		renderButtons[c].texCoords = sf::Vector2f(textures[tId].x + textures[tId].size, textures[tId].y);
		c++;
		renderButtons[c].position = sf::Vector2f(tileSpace + tileSpace * i + tileScale * i, tileY) + sf::Vector2f(tileScale, tileScale);
		renderButtons[c].color = objMap[currentTile][i].getColor();
		renderButtons[c].texCoords = sf::Vector2f(textures[tId].x + textures[tId].size, textures[tId].y + textures[tId].size);
		c++;
		renderButtons[c].position = sf::Vector2f(tileSpace + tileSpace * i + tileScale * i, tileY) + sf::Vector2f(0, tileScale);
		renderButtons[c].color = objMap[currentTile][i].getColor();
		renderButtons[c].texCoords = sf::Vector2f(textures[tId].x, textures[tId].y + textures[tId].size);
		c++;
	}
}

void View::uiUp()
{
	backGroundUi.setPosition(0, upPos);
	close.setPosition(0, upPos);
	cashText.setPosition(window.getSize().x - cashText.getLocalBounds().width - 15, backGroundUi.getPosition().y);
	for (int i = 0; i < sizeof(TileType); i++)
		navButtons[TileType(i)].setPosition((i + 1) * navBarSize.x + (i + 1) * navSpace, upPos);

	down = false;
}

void View::uiDown()
{
	backGroundUi.setPosition(0, downPos);
	close.setPosition(0, downPos);
	cashText.setPosition(window.getSize().x - cashText.getLocalBounds().width - 15, backGroundUi.getPosition().y);
	for (int i = 0; i < sizeof(TileType); i++)
		navButtons[TileType(i)].setPosition((i + 1) * navBarSize.x + (i + 1) * navSpace, downPos);

	down = true;
}

void View::draw()
{
	window.draw(backGroundUi);
	window.draw(close);

	cashText.setString(std::to_string(cash));
	window.draw(cashText);

	if (!down)
	{
		window.draw(renderButtons, &texture);
	}

	for (int i = 0; i < sizeof(TileType); i++)
		window.draw(navButtons[TileType(i)]);
}

View::~View()
{
}