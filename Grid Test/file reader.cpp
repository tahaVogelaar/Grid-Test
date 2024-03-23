#include "file reader.h"

void fileToMap(Map& map, std::string fileName)
{

}

void createTexture(std::unordered_map<TextureId, TextureVarible>& t)
{
	std::pair<TextureId, TextureVarible> pair;

	// box
	pair.first.id = 0;
	pair.first.bitset[0] = true;
	pair.first.bitset[1] = true;
	pair.first.bitset[2] = true;
	pair.first.bitset[3] = true;
	t.insert(pair); // full red box (empty)

	pair.first.id = 1;
	pair.first.bitset[0] = true;
	pair.first.bitset[1] = true;
	pair.first.bitset[2] = true;
	pair.first.bitset[3] = true;
	pair.second.x = 11;
	pair.second.y = 44;
	t.insert(pair); // full black box

	pair.first.bitset[0] = false;
	pair.first.bitset[1] = false;
	pair.first.bitset[2] = false;
	pair.first.bitset[3] = false;
	pair.second.x = 21;
	pair.second.y = 55;
	t.insert(pair); // empty white cell

	// 2 open
	pair.first.bitset[0] = true;
	pair.first.bitset[1] = false;
	pair.first.bitset[2] = false;
	pair.first.bitset[3] = true;
	pair.second.x = 0;
	pair.second.y = 11;
	t.insert(pair); // right down open black

	pair.first.bitset[0] = true;
	pair.first.bitset[1] = true;
	pair.first.bitset[2] = false;
	pair.first.bitset[3] = false;
	pair.second.x = 0;
	pair.second.y = 22;
	t.insert(pair); // left down open black

	pair.first.bitset[0] = false;
	pair.first.bitset[1] = true;
	pair.first.bitset[2] = true;
	pair.first.bitset[3] = false;
	pair.second.x = 0;
	pair.second.y = 33;
	t.insert(pair); // left up open black

	pair.first.bitset[0] = false;
	pair.first.bitset[1] = false;
	pair.first.bitset[2] = true;
	pair.first.bitset[3] = true;
	pair.second.x = 0;
	pair.second.y = 44;
	t.insert(pair); // right up open black

	// 1 open
	pair.first.bitset[0] = true;
	pair.first.bitset[1] = true;
	pair.first.bitset[2] = true;
	pair.first.bitset[3] = false;
	pair.second.x = 11;
	pair.second.y = 0;
	t.insert(pair); // left open black

	pair.first.bitset[0] = true;
	pair.first.bitset[1] = false;
	pair.first.bitset[2] = true;
	pair.first.bitset[3] = true;
	pair.second.x = 11;
	pair.second.y = 11;
	t.insert(pair); // right open black

	pair.first.bitset[0] = false;
	pair.first.bitset[1] = true;
	pair.first.bitset[2] = true;
	pair.first.bitset[3] = true;
	pair.second.x = 11;
	pair.second.y = 22;
	t.insert(pair); // up open black

	pair.first.bitset[0] = true;
	pair.first.bitset[1] = true;
	pair.first.bitset[2] = false;
	pair.first.bitset[3] = true;
	pair.second.x = 11;
	pair.second.y = 33;
	t.insert(pair); // down open black

	pair.first.bitset[0] = true;
	pair.first.bitset[1] = true;
	pair.first.bitset[2] = false;
	pair.first.bitset[3] = true;
	pair.second.x = 11;
	pair.second.y = 33;
	t.insert(pair); // down open black

	// one blocked
	pair.first.bitset[0] = false;
	pair.first.bitset[1] = false;
	pair.first.bitset[2] = false;
	pair.first.bitset[3] = true;
	pair.second.x = 22;
	pair.second.y = 0;
	t.insert(pair); // left blocked

	pair.first.bitset[0] = true;
	pair.first.bitset[1] = false;
	pair.first.bitset[2] = false;
	pair.first.bitset[3] = false;
	pair.second.x = 22;
	pair.second.y = 11;
	t.insert(pair); // up blocked

	pair.first.bitset[0] = false;
	pair.first.bitset[1] = true;
	pair.first.bitset[2] = false;
	pair.first.bitset[3] = false;
	pair.second.x = 22;
	pair.second.y = 22;
	t.insert(pair); // right blocked

	pair.first.bitset[0] = false;
	pair.first.bitset[1] = false;
	pair.first.bitset[2] = true;
	pair.first.bitset[3] = false;
	pair.second.x = 22;
	pair.second.y = 33;
	t.insert(pair); // down blocked

	pair.first.bitset[0] = false;
	pair.first.bitset[1] = true;
	pair.first.bitset[2] = false;
	pair.first.bitset[3] = true;
	pair.second.x = 0;
	pair.second.y = 55;
	t.insert(pair); // 

	pair.first.bitset[0] = true;
	pair.first.bitset[1] = false;
	pair.first.bitset[2] = true;
	pair.first.bitset[3] = false;
	pair.second.x = 11;
	pair.second.y = 55;
	t.insert(pair);


	// kassa
	pair.first.id = 
	// worker
	pair.first.bitset[0] = false;
	pair.first.bitset[1] = false;
	pair.first.bitset[2] = false;
	pair.first.bitset[3] = false;
	pair.second.x = 33;
	pair.second.y = 11;
	t.insert(pair);

	// vertical band
	pair.first.bitset[0] = false;
	pair.first.bitset[1] = false;
	pair.first.bitset[2] = false;
	pair.first.bitset[3] = false;
	pair.second.x = 33;
	pair.second.y = 33;
	t.insert(pair);

	// band up end
	pair.first.bitset[0] = false;
	pair.first.bitset[1] = false;
	pair.first.bitset[2] = false;
	pair.first.bitset[3] = false;
	pair.second.x = 33;
	pair.second.y = 0;
	t.insert(pair);

	// band down end
	pair.first.bitset[0] = false;
	pair.first.bitset[1] = false;
	pair.first.bitset[2] = false;
	pair.first.bitset[3] = false;
	pair.second.x = 33;
	pair.second.y = 22;
	t.insert(pair);
}