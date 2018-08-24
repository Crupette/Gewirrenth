#ifndef PLAYER_H
#define PLAYER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Item.h"

class Player
{
public:
	Player();
	~Player();

	void init(const glm::ivec2& position);
	void destroy();

	void save();
	void load();

	void update();
	void render();

	void setPosition(const glm::ivec2& position);
	glm::ivec2 getPosition() { return m_position; }

	void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	bool isInvincible() { return m_invincibleTicks > 0; }
	bool isObstructed() { return m_obstructed; }

	int getHealth() { return m_health; }
	int getMaxHealth() { return m_maxHealth; }

	Item* getItem(uint8_t slot) { return m_items[slot]; }
	Item* getSelectedItem() { return m_items[m_selectedSlot]; }

	uint8_t getSelectedSlot() { return m_selectedSlot; }

	void destroyItem(uint8_t slot);

	void destroySelectedItem() { destroyItem(m_selectedSlot); }

	bool inventoryFull();
	bool addItem(Item* item);

	void takeDamage(int damage);

private:

	void move(int dx, int dy);

	void createTriangles();

	GLuint m_vao;
	GLuint m_vbo[3];

	glm::ivec2 m_position;
	glm::ivec2 m_direction;

	GLuint m_texture;
	float m_stepTimer;

	int m_health;
	int m_maxHealth;

	float m_invincibleTicks;
	bool m_obstructed;
	bool m_healed;

	Item* m_items[10];
	int8_t m_selectedSlot;

};

#endif