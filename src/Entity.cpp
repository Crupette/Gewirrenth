#include "Entity.h"

#include "FPSRegulator.h"
#include "Base.h"
#include "Level.h"
#include "Player.h"
#include "TextureCache.h"

#include <ctime>

std::mt19937 Entity::m_randomEngine((unsigned int)time(0));

Entity::Entity(const glm::ivec2 & position, const glm::ivec2 & velocity, const glm::ivec4& color, const std::string & texture) : position(position), velocity(velocity), color(color), m_texture(TextureCache::getTexture(texture))
{
}

Entity::Entity(const glm::ivec2 & position, const glm::ivec2 & velocity, const glm::ivec4 & color, GLuint texture) : position(position), velocity(velocity), color(color), m_texture(texture)
{
}

Entity::~Entity()
{
}

bool Entity::tryMove(int dx, int dy)
{
	return Base::getGame()->getLevel()->areaClippable(position.x + dx, position.y + dy);
}

EntityMonster::EntityMonster(int health, int maxHealth, int damage, float speed, const std::string& texture) : Entity(glm::ivec2(0), glm::ivec2(0), glm::ivec4(255), texture), m_health(health), m_maxHealth(maxHealth), m_damage(damage), m_speed(speed), m_invincibleTicks(0.f)
{
}

EntityMonster::~EntityMonster()
{
}

void EntityMonster::update()
{
	m_tick += m_speed * FPSRegulator::getDelta();

	if (m_invincibleTicks > 0) {
		m_invincibleTicks -= 40.f * FPSRegulator::getDelta();
	}

	/*
	if (m_invincibleTicks > 0.f) {
		m_invincibleTicks -= FPSRegulator::getDelta() * 10.f;
		flashTicks++;
		if (flashTicks >= 5) {
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

	*/

	while (m_tick - 1.f > 0) {
		m_tick -= 1.f;
		move();
	}
}

void EntityMonster::onInteract(Item* equipped)
{
	int damage = equipped != nullptr ? equipped->getDamage() : 0;
	printf("Taken %i damage from item %p. Taken down to %i health", damage, equipped, m_health - damage);
	if(equipped != nullptr) equipped->onUse(this);
	if (m_invincibleTicks <= 0.f) {
		m_invincibleTicks = 1.f;

		m_health -= damage;
		if (m_health <= 0) {
			onKill();
			printf("\n");
			return;
		}
		return;
	}
	printf(" INVINCIBLE, %f ticks remaining\n", m_invincibleTicks);
}

void EntityMonster::move()
{
	Player* player = Base::getGame()->getPlayer();
	Level* level = Base::getGame()->getLevel();

	glm::ivec2 distance = player->getPosition() - position;

	int dirX = distance.x >= 0 ? 1 : -1;
	int dirY = distance.y >= 0 ? 1 : -1;

	if ((player->isInvincible() && !player->isObstructed()) && m_invincibleTicks <= 0.f) {
		dirX = -dirX;
		dirY = -dirY;
	}

	if (player->getHealth() == 0) {
		std::uniform_int_distribution<int> randDir(-1, 1);
		dirX = randDir(m_randomEngine);
		dirY = randDir(m_randomEngine);
	}

	bool switched = false;

	if (abs(distance.x) > abs(distance.y)) {
	entitymoveX:
		if (tryMove(dirX, 0)) {
			position.x += dirX;
		}
		else {
			if (player->getPosition() == position + glm::ivec2(dirX, 0)) {
				printf("Player at %i, %i took %i damage from entity %p with %i health: ", position.x + dirX, position.y + dirY, m_damage, this, m_health);
				player->takeDamage(m_damage);
			}
			else {
				if (!switched) {
					switched = true;
					goto entitymoveY;
				}
			}
		}
	}
	else {
	entitymoveY:
		if (tryMove(0, dirY)) {
			position.y += dirY;
		}
		else {
			if (player->getPosition() == position + glm::ivec2(0, dirY)) {
				printf("Player at %i, %i took %i damage from entity %p with %i health: ", position.x + dirX, position.y + dirY, m_damage, this, m_health);
				player->takeDamage(m_damage);
			}
			else {
				if (!switched) {
					switched = true;
					goto entitymoveX;
				}
			}
		}
	}
}

void EntityMonster::onKill()
{
	Player* player = Base::getGame()->getPlayer();
	std::uniform_int_distribution<int> m_dropChance(1, 100);

	for (auto& it : m_lootTable) {
		unsigned int drop = m_dropChance(m_randomEngine);
		if (drop >= it.first.first && drop <= it.first.second) {
			Item* item = ItemSelector::getItem(it.second);
			if (!player->addItem(item)) {
				std::uniform_int_distribution<int> veldist(-10, 10);
				Base::getGame()->getLevel()->addEntity(new EntityItem(item, position, glm::ivec2(veldist(m_randomEngine), veldist(m_randomEngine))));
			 }
		}
	}
	Base::getGame()->getLevel()->enemyKilled();
	Base::getGame()->getLevel()->killEntity(this);
}

EntityItem::EntityItem(Item* item, const glm::ivec2& position, const glm::ivec2& velocity) : Entity(position, velocity, item->getColor(), item->getTexture()), m_item(item) {}

void EntityItem::update()
{
	m_tick += FPSRegulator::getDelta() * ((abs(velocity.x) + abs(velocity.y))/2.f);
	//printf("Item \"%s\" has velocity %i, %i. Tick %f\n", m_item->getName().c_str(), velocity.x, velocity.y, m_tick);
	if (m_tick >= 1.f) {
		m_tick -= 1.f;
		glm::ivec2 direction = glm::ivec2(velocity.x > 0 ? 1 : (velocity.x < 0 ? -1 : 0), velocity.y > 0 ? 1 : (velocity.y < 0 ? -1 : 0));
		if (tryMove(direction.x, direction.y)) {
			position += direction;
			velocity /= 4;
		}
	}
}

void EntityItem::onInteract(Item * equipped)
{
	Player* player = Base::getGame()->getPlayer();
	if (player->addItem(m_item)) {
		m_item = nullptr;
		destroy();
	}
}

void EntityItem::destroy()
{
	clean();
	Base::getGame()->getLevel()->killEntity(this);
}

void EntityItem::clean() {
	if (m_item != nullptr) {
		delete m_item;
	}
}

Elf::Elf() : EntityMonster(1, 1, 1, 70.f, "textures/Elf.png"){
	m_lootTable.emplace(std::make_pair(1, 5), 3);
	m_lootTable.emplace(std::make_pair(1, 10), 1);
	m_lootTable.emplace(std::make_pair(1, 20), 2);
}

BossElf::BossElf() : EntityMonster(5, 5, 1, 40.f, "textures/Elf.png")
{
	color = glm::ivec4(255, 128, 128, 255);
	m_lootTable.emplace(std::make_pair(1, 100), 5);
}


Goblin::Goblin() : EntityMonster(4, 4, 2, 20.f, "textures/Goblin.png")
{
	m_lootTable.emplace(std::make_pair(1, 20), 2);
	m_lootTable.emplace(std::make_pair(1, 25), 4);
}

BossGoblin::BossGoblin() : EntityMonster(10, 10, 10, 20.f, "textures/Goblin.png")
{
	color = glm::ivec4(255, 128, 128, 255);
	m_lootTable.emplace(std::make_pair(1, 100), 6);
}


Troll::Troll() : EntityMonster(3, 3, 1, 60.f, "textures/TEnemy.png")
{
	color = glm::ivec4(128, 255, 255, 255);
	m_lootTable.emplace(std::make_pair(1, 50), 3);
	m_lootTable.emplace(std::make_pair(1, 20), 7);
}

BossTroll::BossTroll() : EntityMonster(20, 20, 2, 50.f, "textures/TEnemy.png")
{
	color = glm::ivec4(0, 128, 128, 255);
	m_lootTable.emplace(std::make_pair(1, 100), 8);
}
