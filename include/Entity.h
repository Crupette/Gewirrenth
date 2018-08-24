#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <random>
#include <map>
#include <utility>

class Item;

class Entity
{
public:
	Entity(const glm::ivec2& position, const glm::ivec2& velocity, const glm::ivec4& color, const std::string& texture);
	Entity(const glm::ivec2& position, const glm::ivec2& velocity, const glm::ivec4& color, GLuint texture);
	~Entity();

	virtual void update() = 0;

	virtual void onInteract(Item* equipped) = 0;
	virtual void destroy() {}

	virtual void clean() {}

	GLuint getTexture() { return m_texture; }

	glm::ivec2 position;
	glm::ivec2 velocity;

	glm::ivec4 color;

protected:

	bool tryMove(int dx, int dy);

	GLuint m_texture;

	float m_tick;

	static std::mt19937 m_randomEngine;
};

class EntityMonster : public Entity {
public:
	EntityMonster(int health, int maxHealth, int damage, float speed, const std::string& texture);
	~EntityMonster();

	virtual void update();

	virtual void onInteract(Item* equipped);

protected:
	void move();

	virtual void onKill();

	int m_health, m_maxHealth, m_damage;
	float m_speed, m_invincibleTicks;

	std::map<std::pair<unsigned int, unsigned int>, unsigned int> m_lootTable;
};

class EntityItem : public Entity {
public:
	EntityItem(Item* item, const glm::ivec2& position, const glm::ivec2& velocity);
	~EntityItem() {}
	
	void update();

	void onInteract(Item* equipped);
	void destroy();
	void clean();

protected:
	Item * m_item;
};

class Elf : public EntityMonster {
public:
	Elf();
	~Elf() {}
};

class BossElf : public EntityMonster {
public:
	BossElf();
	~BossElf() {}
};

class Goblin : public EntityMonster {
public:
	Goblin();
	~Goblin() {}
};

class BossGoblin : public EntityMonster {
public:
	BossGoblin();
	~BossGoblin() {}
};

class Troll : public EntityMonster {
public:
	Troll();
	~Troll() {}
};

class BossTroll : public EntityMonster {
public:
	BossTroll();
	~BossTroll() {}
};

namespace EntitySelector {

	static int getNumEntities() {
		return 3;
	}

	static Entity* getBossEntity(unsigned int id) {
		switch (id)
		{
		case 0:
			return new BossElf();
		case 1:
			return new BossGoblin();
		case 2:
			return new BossTroll();
		default:
			return nullptr;
		}
	}

	static Entity* getEntity(unsigned int id) {
		switch (id)
		{
		case 0:
			return new Elf();
		case 1:
			return new Goblin();
		case 2:
			return new Troll();
		default:
			return nullptr;
		}
	}
}

#endif