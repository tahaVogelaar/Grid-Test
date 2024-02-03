#pragma once
#include <bitset>
#include <unordered_map>

struct TextureId {
	std::bitset<4> bitset;
	int id = 0;

	TextureId(const bool a) 
	{
		bitset[0] = a;
		bitset[1] = a;
		bitset[2] = a;
		bitset[3] = a;
	}

	TextureId()
	{
		bitset[0] = true;
		bitset[1] = true;
		bitset[2] = true;
		bitset[3] = true;
	}

	bool operator==(const TextureId& other) const
	{
		return (bitset == other.bitset) && (id == other.id);
	}
};

struct TextureVarible {
	int size = 9;
	int x = 0, y = 0;
};

namespace std {
	template <>
	struct hash<TextureId>
	{
		size_t operator()(const TextureId& key) const
		{
			const std::size_t phi = 0x9e3779b9;
			size_t hashValue = std::hash<std::bitset<4>>()(key.bitset);
			hashValue ^= std::hash<int>()(key.id) + phi + (hashValue << 6) + (hashValue >> 2);
			return hashValue;
		}
	};
}