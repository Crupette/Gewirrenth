#ifndef TILEREGISTRY_H
#define TILEREGISTRY_H

#include "Tile.h"

#include <unordered_map>

class TileRegistry {
public:
	
	static void init();
	static void destroy();

	static Tile* getTileFromId(unsigned int id);
	static unsigned int getIdFromTile(Tile* tile);

private:

	static std::unordered_map<unsigned int, Tile*> m_tiles;
};

#endif