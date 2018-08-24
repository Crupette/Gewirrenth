#ifndef ENTITYRENDERER_H
#define ENTITYRENDERER_H

#include "Entity.h"
#include "SpriteBatch.h"

class EntityRenderer
{
public:
	
	static void init();

	static void addEntity(Entity* entity);

	static void render();

private:
	
	static SpriteBatch m_batch;

	static std::vector<Entity*> m_entities;
};

#endif