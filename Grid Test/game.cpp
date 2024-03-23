#include "game.h"
#include "map.h"
#include "npc.h"
#include "view.h"
#include <iostream>
#include <chrono>
#include "texture.h"
#include "file reader.h"

const sf::Vector2i windowSize(1000, 1000);
int viewGridSize = 16;

sf::Font font;
const float BUiRatio = .25f;
std::unordered_map<TextureId, TextureVarible> textures;

int game()
{
	sf::Texture texture;
	// load the game files
	font.loadFromFile("extentions/OpenSans-Bold.ttf");
	texture.loadFromFile("extentions/grid textures.png");
	createTexture(textures); // load textures
	// delta time
	std::chrono::steady_clock::time_point begin;
	float deltaTime = 0;

	// gameplay
	sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "grid game");
	Map map(window, viewGridSize, textures, "game map", texture);

	std::vector<Npc> players;
	for (size_t i = 0; i < 1; i++)
	{
		players.push_back(Npc(map, deltaTime));
	}
	View view(window, map.getTileSize(), texture);
	map.cashPtr = &view.cash;

	while (window.isOpen())
	{
		begin = std::chrono::steady_clock::now();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				map.~Map();
			}
			view.updateUi(event);
		}

		window.clear();

		bool b = view.update(deltaTime);
		map.update(view.getPosition(), event, b, *view.getCurrentObj());

		map.draw();

		for (auto& i : players)
		{
			i.update();
			i.getPath().draw(window, map.getTileSize(), view.getPosition());
		}

		for (auto& i : players)
		{
			i.draw(window, view.getPosition());
		}

		view.draw();

		window.display();

		deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count()) / 3000;
	}
	return 0;
}
