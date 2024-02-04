#include "view.h"
#include "game.h"
#include <iostream>
#include <algorithm>

View::View(sf::RenderWindow& w, float cellSize) : window(w), scale(std::max(window.getSize().x, window.getSize().y) / 100), down(true), renderButtons(sf::PrimitiveType::Quads),
navBarSize(sf::Vector2f(5 * scale, 2.5f * scale)), tileScale(navBarSize.x * 2), tileSpace(tileScale / 4)
{
	backGroundUi.setSize(sf::Vector2f(window.getSize().x, window.getSize().y * BUiRatio));
	close.setSize(navBarSize);
	close.setFillColor(sf::Color::Red);

	downPos = window.getSize().y - close.getSize().y;
	upPos = window.getSize().y - backGroundUi.getSize().y;
	buttonTop = upPos - (scale / 3);

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
	GameObject b1(0, 0, -50, sf::Color::White, TileType::customer);
	objMap[b1.getTileType()].push_back(b1);

	GameObject b2(0, 0, -50, sf::Color::White, TileType::EandC);
	objMap[b2.getTileType()].push_back(b2);

	GameObject b3(0, 0, -50, sf::Color::White, TileType::blocked);
	objMap[b3.getTileType()].push_back(b3);

	GameObject b4(0, 0, -50, sf::Color::White, TileType::employe);
	objMap[b4.getTileType()].push_back(b4);

	std::vector<GameObject*> v;
	GameObject* heigest = nullptr;
	for (auto& i : objMap)
	{
		//std::sort(i.second.begin(), i.second.end(), &GameObject::compareByCost); // error
		for (int j = 0; j < i.second.size(); j++)
		{
			i.second[j].setOrder(j);
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

	if (mousePos.y > downPos) // ui up
		uiUp();
	if (mousePos.x >= close.getPosition().x && // ui down
		mousePos.x <= close.getPosition().x + close.getSize().x &&
		mousePos.y >= close.getPosition().y &&
		mousePos.y <= close.getPosition().y + close.getSize().y)
		uiDown();

	if (!down && backGroundUi.getPosition().y < mousePos.y && mousePos.y < window.getSize().y)
	{
		return false;
	}
	if (down)
		return false;
	return true;
}

void View::selectTileType(TileType t) // make the textures for the selected stuff or what ever am too tired to think and do something productif or simple, my brain is basicly disabled because of it. i may need some sleep and stop wriding this trash, my mom wants me to come to her and do something together but am just wriding this instad for no reason. i guess am just bored and sould leave my pc and go spend some time with my mom, but on the other end i just want to write this stupid thing. am also wriding without looking at my keyboard and it makes me feel cool, maybe thats why i dont want to leave my pc. i really gotta go tho. good night to who ever is gonna read this. i may just extend this. - 00:30 04/02/2024
{
	int c = 0;
	TextureId tId(true);
	for (int i = 0; i < objMap[t].size(); i++)
	{
		tId.id = objMap[t][i].getTextureId();

		renderButtons[c].position = sf::Vector2f(tileSpace + tileSpace * i + tileScale * i, buttonTop);
		renderButtons[c].color = objMap[t][i].getColor();
		renderButtons[c].texCoords = sf::Vector2f(textures[tId].x, textures[tId].y);
		c++;
		renderButtons[c].position = sf::Vector2f(tileSpace + tileSpace * i + tileScale * i, buttonTop) + sf::Vector2f(tileScale, 0);
		renderButtons[c].color = objMap[t][i].getColor();
		renderButtons[c].texCoords = sf::Vector2f(textures[tId].x + textures[tId].size, textures[tId].y);
		c++;
		renderButtons[c].position = sf::Vector2f(tileSpace + tileSpace * i + tileScale * i, buttonTop) + sf::Vector2f(tileScale, tileScale);
		renderButtons[c].color = objMap[t][i].getColor();
		renderButtons[c].texCoords = sf::Vector2f(textures[tId].x + textures[tId].size, textures[tId].y + textures[tId].size);
		c++;
		renderButtons[c].position = sf::Vector2f(tileSpace + tileSpace * i + tileScale * i, buttonTop) + sf::Vector2f(0, tileScale);
		renderButtons[c].color = objMap[t][i].getColor();
		renderButtons[c].texCoords = sf::Vector2f(textures[tId].x, textures[tId].y + textures[tId].size);
		c++;
	}
}

void View::uiUp()
{
	backGroundUi.setPosition(0, upPos);
	close.setPosition(0, upPos);
	cashText.setPosition(window.getSize().x - cashText.getLocalBounds().width - 15, backGroundUi.getPosition().y);
	down = false;
}

void View::uiDown()
{
	backGroundUi.setPosition(0, downPos);
	close.setPosition(0, downPos);
	cashText.setPosition(window.getSize().x - cashText.getLocalBounds().width - 15, backGroundUi.getPosition().y);
	down = true;
}

void View::draw()
{
	window.draw(backGroundUi);
	window.draw(close);

	cashText.setString(std::to_string(cash));
	window.draw(cashText);

	if (!down)
		window.draw(renderButtons, &texture);
}

View::~View()
{
}