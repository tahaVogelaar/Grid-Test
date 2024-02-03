#include "view.h"
#include "game.h"
#include <iostream>

View::View(sf::RenderWindow& w, float cellSize) : window(w), scale(std::max(window.getSize().x, window.getSize().y) / 100), down(true)
{
	moveSpeed;

	backGroundUi.setSize(sf::Vector2f(window.getSize().x, window.getSize().y * BUiRatio));
	close.setSize(sf::Vector2f(5 * scale, 2.5f * scale));
	close.setFillColor(sf::Color::Red);

	downPos = window.getSize().y - close.getSize().y;
	upPos = window.getSize().y - backGroundUi.getSize().y;
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
	gameObjects.push_back(GameObject(0, 0, -50, sf::Color::White));
	gameObjects.push_back(GameObject(1, 0, 100, sf::Color::Blue));
	gameObjects.push_back(GameObject(1, 1, 100, sf::Color::Green));
	gameObjects.push_back(GameObject(1, 2, 1, sf::Color(126, 126, 126)));
	currentObj = &gameObjects[1];
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

	if (mousePos.y > downPos) // ui up
	{
		backGroundUi.setPosition(0, upPos);
		close.setPosition(0, upPos);
		cashText.setPosition(window.getSize().x - cashText.getLocalBounds().width - 15, backGroundUi.getPosition().y);
		down = false;
	}
	if (mousePos.x >= close.getPosition().x && // ui down
		mousePos.x <= close.getPosition().x + close.getSize().x &&
		mousePos.y >= close.getPosition().y &&
		mousePos.y <= close.getPosition().y + close.getSize().y)
	{
		backGroundUi.setPosition(0, downPos);
		close.setPosition(0, downPos);
		cashText.setPosition(window.getSize().x - cashText.getLocalBounds().width - 15, backGroundUi.getPosition().y);
		down = true;
	}

	if (!down && backGroundUi.getPosition().y < mousePos.y && mousePos.y < window.getSize().y)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) // pressing one of the game objects
		{
			for (auto& i : gameObjects)
			{
				if (mousePos.x >= i.shape.getPosition().x &&
					mousePos.x <= i.shape.getPosition().x + i.shape.getSize().x &&
					mousePos.y >= i.shape.getPosition().y &&
					mousePos.y <= i.shape.getPosition().y + i.shape.getSize().y)
				{
					currentObj = &i;
				}
			}
		}

		return false;
	}
	if (down)
		return false;
	return true;
}

void View::draw()
{
	window.draw(backGroundUi);
	window.draw(close);

	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i].shape.setPosition((i*1.25f) * gameObjects[i].shape.getSize().x + 5, backGroundUi.getPosition().y + 50);
		window.draw(gameObjects[i].shape);
	}

	cashText.setString(std::to_string(cash));
	window.draw(cashText);
}

View::~View()
{
}