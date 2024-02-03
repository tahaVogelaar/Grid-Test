#pragma once
#include "texture.h"
#include "map.h"
#include <fstream>
#include <iostream>

void fileToMap(Map& map, std::string fileName);

void createTexture(std::unordered_map<TextureId, TextureVarible>& t);