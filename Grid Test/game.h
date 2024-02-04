#pragma once
#include "SFML/Graphics.hpp"
#include <unordered_map>
#include "texture.h"

#ifndef GAME_H
#define GAME_H
extern sf::Font font;
extern sf::Texture texture;
extern const float BUiRatio; // background ui ratio compared to the windowY size
extern std::unordered_map<TextureId, TextureVarible> textures;
#endif // GAME_H

int game();

