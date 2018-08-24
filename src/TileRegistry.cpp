#include "TileRegistry.h"

#include "TileDoor.h"

#include <algorithm>

std::unordered_map<unsigned int, Tile*> TileRegistry::m_tiles;

void TileRegistry::init()
{
	m_tiles.emplace(0, new Tile(0, true, "textures/Wall.png"));
	m_tiles.emplace(1, new Tile(1, false, "textures/Floor.png"));
	m_tiles.emplace(2, new Tile(2, true, "textures/Rubble.png"));
	m_tiles.emplace(3, new TileDoor(3));
	m_tiles.emplace(4, new Tile(4, true, "textures/Blank.png"));
}

void TileRegistry::destroy()
{
	for (std::unordered_map<unsigned int, Tile*>::iterator it = m_tiles.begin(); it != m_tiles.end();) {
		delete it->second;
		it = m_tiles.erase(it);
	}
}

Tile * TileRegistry::getTileFromId(unsigned int id)
{
	auto it = m_tiles.find(id);
	if (it == m_tiles.end()) {
		return nullptr;
	}
	return it->second;
}

unsigned int TileRegistry::getIdFromTile(Tile * tile)
{
	for (auto& it : m_tiles) {
		if (it.second == tile) {
			return it.first;
		}
	}
	return 0;
}
