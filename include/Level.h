#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

#include "Tile.h"
#include "SpriteBatch.h"
#include "TileRenderer.h"
#include "FontRenderer.h"
#include "Entity.h"

class Level
{
public:
	Level();
	~Level();

	void init();

	void generateLevel(unsigned int newId);
	void saveLevel();
	void loadLevel(unsigned int id);

	void update();
	void render();

	Tile* getTile(int x, int y);
	Entity* getEntity(int x, int y);
	bool areaClippable(int x, int y);

	void addEntity(Entity* entity);
	void killEntity(Entity* entity);

	void enemyKilled() { m_numEnemies--; }

	void enterDoorFrom(glm::ivec2& position);

	unsigned int getLevelId() { return m_id; }
	unsigned int getLevel() { return m_level; }
	unsigned int getNextId() { return m_nextId; }
	void setLevelAfterTitle(unsigned int id) { m_currentLevel = id; }

	void setLevel(unsigned int level) { m_level = level; }
	void setNextId(unsigned int id) { m_nextId = id; }

	bool levelCleared() { return m_cleared; }

	std::pair<unsigned int, unsigned int> getDimensions() { return std::make_pair(m_width, m_height); }

private:

	void renderTiles();

	void cleanData();

	void generateBossLevel();
	
	std::vector<Tile*> m_tiles;
	std::vector<Entity*> m_entities;

	std::vector < std::pair<unsigned int, unsigned int>> m_doors;

	unsigned int m_id;
	unsigned int m_level;
	unsigned int m_currentLevel;

	unsigned int m_nextId;
	unsigned int m_numEnemies;

	size_t m_width;
	size_t m_height;

	bool m_cleared;
	bool m_boss;

	bool m_persistWidth;
	bool m_persistHeight;

	glm::ivec2 m_enterPos;

	static std::mt19937 m_randomEngine;

	//Title screen stuff
	void generateTitleScreen();
};

#endif

