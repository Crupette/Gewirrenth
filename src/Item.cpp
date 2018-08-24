#include "Item.h"

#include "TextureCache.h"
#include "Base.h"
#include "Game.h"
#include "Player.h"

Item::Item(int id, const std::string & name, const std::string & description, GLuint texture, int damage, const glm::ivec4 & color) :
	m_name(name), m_description(description), m_texture(texture), m_damage(damage), m_color(color), id(id)
{

}

Item::~Item()
{
}

void Item::onEquip()
{
	
}

void Item::load(std::ifstream & file)
{
	file.read((char*)(&m_damage), 4);
}

void Item::save(std::ofstream & file)
{
	file.write((char*)(&m_damage), 4);
}

ItemSword::ItemSword(int id, const std::string & name, const std::string & description, const std::string & texture, int damage, const glm::ivec4 & color) : Item(id, name, description, TextureCache::getTexture(texture), damage, color) {}

ItemPotion::ItemPotion(int id, const std::string & name, const std::string & description, const std::string & texture, int healing, const glm::ivec4 & color) : Item(id, name, description, TextureCache::getTexture(texture), healing, color) {}

void ItemPotion::onEquip()
{
	Player* player = Base::getGame()->getPlayer();
	player->takeDamage(-m_damage);
	player->destroySelectedItem();
}

void ItemPotion::onUse(Entity * entity)
{
	Player* player = Base::getGame()->getPlayer();
	player->destroySelectedItem();
}

ItemFlask::ItemFlask(int id, const std::string & name, const std::string & description, const std::string & texture, int healing, int uses, const glm::ivec4 & color) : Item(id, name, description, TextureCache::getTexture(texture), healing, color), m_uses(uses)
{
}

void ItemFlask::onEquip()
{
	Player* player = Base::getGame()->getPlayer();
	player->takeDamage(-m_damage);
	if (m_uses-- <= 0) {
		player->destroySelectedItem();
	}
}

void ItemFlask::onUse(Entity * entity)
{
	Player* player = Base::getGame()->getPlayer();
	if (m_uses-- <= 0) {
		player->destroySelectedItem();
	}
}

void ItemFlask::load(std::ifstream & file)
{
	file.read((char*)(&m_damage), 4);
	file.read((char*)(&m_uses), 4);
}

void ItemFlask::save(std::ofstream & file)
{
	file.write((char*)(&m_damage), 4);
	file.write((char*)(&m_uses), 4);
}
