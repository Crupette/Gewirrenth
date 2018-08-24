#ifndef TILERENDERER_H
#define TILERENDERER_H

#include "SpriteBatch.h"
#include "Tile.h"

class TileRenderer
{
public:
	static void init();
	static void destroy();

	static void addTilesForBatching(std::vector<Tile*> tiles, size_t width, size_t height);

	static void render();

private:
	static SpriteBatch m_batch;
};

#endif