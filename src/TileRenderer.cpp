#include "TileRenderer.h"

SpriteBatch TileRenderer::m_batch;

void TileRenderer::init()
{
	m_batch.init();
}

void TileRenderer::destroy()
{
	m_batch.destroy();
}

void TileRenderer::addTilesForBatching(std::vector<Tile*> tiles, size_t width, size_t height)
{
	m_batch.begin();

	for(size_t y = 0; y < height; y++)
	for(size_t x = 0; x < width; x++){
		size_t index = x + y * width;
		m_batch.addSprite(glm::vec4(x * 100, y * 100, 100, 100), glm::vec4(0, 0, 1, 1), glm::ivec4(255, 255, 255, 255),0, tiles[index]->getTexture());
	}

	m_batch.end();
}

void TileRenderer::render()
{
	m_batch.renderBatches();
}
