#include "Player.h"
#include "TextureCache.h"
#include "FPSRegulator.h"
#include "InputManager.h"

#include "Base.h"
#include "Game.h"
#include "Level.h"
#include "Tile.h"
#include "RoguelikeMath.h"
#include "ErrorHandler.h"

#include <SDL2/SDL.h>
#include <vector>
#include <fstream>

Player::Player() : m_vao(0), m_health(0), m_maxHealth(0), m_selectedSlot(0), m_healed(false), m_direction(0)
{
	for (int i = 0; i < 10; i++) {
		m_items[i] = nullptr;
	}
}


Player::~Player()
{
}

void Player::init(const glm::ivec2& position)
{
	m_health = 20;
	m_maxHealth = 20;

	glGenVertexArrays(1, &m_vao);

	glBindVertexArray(m_vao);

	glGenBuffers(3, m_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	std::vector<float> uvs =
	{
		0, 0,
		1, 0,
		0, 1,
		1, 0,
		0, 1,
		1, 1
	};

	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_texture = TextureCache::getTexture("textures/Player.png");
	setPosition(position);
	setColor(255, 255, 255, 255);
}

void Player::destroy()
{
	for (int i = 0; i < 10; i++) {
		delete m_items[i];
	}

	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(3, m_vbo);
}

void Player::save()
{
	std::ofstream file("save/player.dat", std::ios::binary);

	if (!file) {
		fatalError("Player: Failed to load file \"save/player.dat\".");
	}

	unsigned int nextId = Base::getGame()->getLevel()->getNextId();
	file.write((char*)(&nextId), 4);

	unsigned int currentLevel = Base::getGame()->getLevel()->getLevelId();
	file.write((char*)(&currentLevel), 4);

	unsigned int otherLevel = Base::getGame()->getLevel()->getLevel();
	file.write((char*)(&otherLevel), 4);

	file.write((char*)(&m_position.x), 4);
	file.write((char*)(&m_position.y), 4);

	for (Item* item : m_items) {
		if (item == nullptr) {
			unsigned int noitem = 0xFFFFFFFF;
			file.write((char*)(&noitem), 4);
			continue;
		}
		file.write((char*)(&item->id), 4);
		item->save(file);
	}

	file.write((char*)(&m_health), 4);
	file.write((char*)(&m_maxHealth), 4);
}

void Player::load()
{
	std::ifstream file("save/player.dat");

	if (file.fail()) {
		m_items[0] = ItemSelector::getItem(0);
		setPosition(glm::ivec2(1, 1));
		return;
	}

	unsigned int nextId;
	file.read((char*)(&nextId), 4);
	Base::getGame()->getLevel()->setNextId(nextId);

	file.read((char*)(&nextId), 4);
	Base::getGame()->getLevel()->setLevelAfterTitle(nextId);

	file.read((char*)(&nextId), 4);
	Base::getGame()->getLevel()->setLevel(nextId);

	file.read((char*)(&m_position.x), 4);
	file.read((char*)(&m_position.y), 4);

	setPosition(getPosition());
	
	for (size_t i = 0; i < 10; i++) {
		if (m_items[i] != nullptr) {
			delete m_items[i];
			m_items[i] = nullptr;
		}

		unsigned int id;
		file.read((char*)(&id), 4);
		if (id == 0xFFFFFFFF) {
			continue;
		}
		m_items[i] = ItemSelector::getItem(id);
		if (m_items[i] == nullptr) continue;
		m_items[i]->load(file);
	}

	file.read((char*)(&m_health), 4);
	file.read((char*)(&m_maxHealth), 4);
}

void Player::update()
{
	m_stepTimer += 100.f * FPSRegulator::getDelta();

	static int flashTicks = 0;

	m_selectedSlot -= InputManager::getScrollWheelMovement();
	if (m_selectedSlot < 0) m_selectedSlot = 9;
	if (m_selectedSlot >= 10) m_selectedSlot = 0;

	if (InputManager::isKeyDown(SDL_BUTTON_LEFT)) {
		if (m_items[m_selectedSlot] != nullptr && m_invincibleTicks <= 0.f) {
			m_items[m_selectedSlot]->onEquip();
		}
	}

	if (InputManager::isKeyDown(SDLK_q)) {
		if (m_items[m_selectedSlot] != nullptr) {
			Base::getGame()->getLevel()->addEntity(new EntityItem(m_items[m_selectedSlot], m_position, m_direction * glm::ivec2(100)));
			m_items[m_selectedSlot] = nullptr;
		}
	}

	if (m_invincibleTicks > 0.f) {
		m_invincibleTicks -= FPSRegulator::getDelta() * 20.f;
		flashTicks++;
		if (flashTicks >= 5){
			flashTicks = -5;
			setColor(255, 0, 0, 255);
		}
		else if (flashTicks == 0) {
			setColor(255, 255, 255, 255);
		}
	}
	else if (flashTicks <= 0 || flashTicks >= 0) {
		flashTicks = 0;
		setColor(255, 255, 255, 255);
	}

	if (m_health == 0) {
		setColor(128, 0, 0, 255);
		for (int i = 0; i < 10; i++) {
			if (m_items[i] != nullptr) {
				Base::getGame()->getLevel()->addEntity(new EntityItem(m_items[i], m_position, m_direction * 10));
				m_items[i] = nullptr;
			}
		}
	}

	while (m_stepTimer - 1.f > 0) {
		m_stepTimer -= 1.f;

		if (InputManager::isKeyDown(SDLK_w)) { move(0, 1); m_direction = glm::ivec2(0, 1); }
		if (InputManager::isKeyDown(SDLK_s)) { move(0, -1); m_direction = glm::ivec2(0, -1); }
		if (InputManager::isKeyDown(SDLK_d)) { move(1, 0); m_direction = glm::ivec2(1, 0); }
		if (InputManager::isKeyDown(SDLK_a)) { move(-1, 0); m_direction = glm::ivec2(-1, 0); }

		/*
			Smart AI Help
		*/

		Level* level = Base::getGame()->getLevel();

		int obstructions = 0;
		//if (!level->areaClippable(m_position.x + 1, m_position.y)) obstructions++;
		//if (!level->areaClippable(m_position.x - 1, m_position.y)) obstructions++;
		//if (!level->areaClippable(m_position.x, m_position.y + 1)) obstructions++;
		//if (!level->areaClippable(m_position.x, m_position.y - 1)) obstructions++;

		if (obstructions >= 2) {
			m_obstructed = true;
		}
		else {
			m_obstructed = false;
		}
	}
}

void Player::render()
{
	glBindVertexArray(m_vao);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
}

void Player::setPosition(const glm::ivec2 & position)
{
	m_position = position;
	createTriangles();
}

void Player::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{

	std::vector<unsigned char> colors = 
	{
		r, g, b, a,
		r, g, b, a,
		r, g, b, a,
		r, g, b, a,
		r, g, b, a,
		r, g, b, a
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);

	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(char), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, colors.size() * sizeof(char), colors.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Player::destroyItem(uint8_t slot)
{
	if (m_items[slot] != nullptr) {
		delete m_items[slot];
	}
	m_items[slot] = nullptr;
}

bool Player::inventoryFull()
{
	for (uint8_t i = 0; i < 10; i++) {
		if (m_items[i] == nullptr) {
			return false;
		}
	}
	return true;
}

bool Player::addItem(Item * item)
{
	for (uint8_t i = 0; i < 10; i++) {
		if (m_items[i] == nullptr) {
			m_items[i] = item;
			return true;
		}
	}
	return false;
}

void Player::takeDamage(int damage)
{
	printf("Player %p took %i damage at %i, %i\n", this, damage, m_position.x, m_position.y);
	if (m_health == 0) return;
	if (damage < 0) {
		m_health = m_health - damage > m_maxHealth ? m_maxHealth : m_health - damage;
		m_invincibleTicks = 1.f;
	}else if (m_invincibleTicks <= 0) {
		m_health -= damage;

		m_invincibleTicks = 1.f;
		if (m_health <= 0) {
			m_health = 0;
			m_invincibleTicks = 0.f;
		}
	}
}

void Player::move(int dx, int dy)
{
	if (m_health == 0) return;
	Tile* tile = Base::getGame()->getLevel()->getTile(m_position.x + dx, m_position.y + dy);
	if (tile != nullptr) {
		if (Base::getGame()->getLevel()->areaClippable(m_position.x + dx, m_position.y + dy)) {
			setPosition(getPosition() + glm::ivec2(dx, dy));
			tile->collide();
			return;
		}
		tile->collide();
	}
	if (!Base::getGame()->getLevel()->areaClippable(m_position.x + dx, m_position.y + dy)) {
		Entity* entity = Base::getGame()->getLevel()->getEntity(m_position.x + dx, m_position.y + dy);
		if (entity != nullptr) {
			printf("Player %p interacting with entity %p with item: (%s, %i damage): ",this, entity, m_items[m_selectedSlot] == nullptr ? "Nothing" : m_items[m_selectedSlot]->getName().c_str(), m_items[m_selectedSlot] == nullptr ? 0 : m_items[m_selectedSlot]->getDamage());
			entity->onInteract(m_items[m_selectedSlot]);
			return;
		} else {
			return;
		}
	}
	
}

void Player::createTriangles()
{
	std::vector<float> positions =
	{
		(m_position.x * 100.f), (m_position.y * 100.f),
		(m_position.x * 100.f) + 100, (m_position.y * 100.f),
		(m_position.x * 100.f), (m_position.y * 100.f) + 100,
		(m_position.x * 100.f) + 100, (m_position.y * 100.f),
		(m_position.x * 100.f), (m_position.y * 100.f) + 100,
		(m_position.x * 100.f) + 100, (m_position.y * 100.f) + 100
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(float), positions.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
