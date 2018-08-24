#include "EntityRenderer.h"

SpriteBatch EntityRenderer::m_batch;
std::vector<Entity*> EntityRenderer::m_entities;


void EntityRenderer::init()
{
	m_batch.init();
}

void EntityRenderer::addEntity(Entity * entity)
{
	m_entities.push_back(entity);
}

void EntityRenderer::render()
{
	m_batch.begin();

	for (Entity* entity : m_entities) {
		m_batch.addSprite(glm::vec4(entity->position.x * 100.f, entity->position.y * 100.f, 100, 100), glm::vec4(0, 0, 1, 1), entity->color, 0, entity->getTexture());
	}

	m_batch.end();

	m_entities.clear();

	m_batch.renderBatches();
}
