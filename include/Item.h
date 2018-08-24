#ifndef ITEM_H
#define ITEM_H

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <fstream>

#include "Entity.h"

class Item
{
public:
	Item( int id, const std::string& name, const std::string& description, GLuint texture, int damage, const glm::ivec4& color);
	virtual ~Item();

	virtual void onUse(Entity* entity) {}
	virtual void onEquip();

	std::string getName() { return m_name; }
	std::string getDescription() { return m_description; }

	virtual int getDamage() { return m_damage; }

	GLuint getTexture() { return m_texture; }

	glm::ivec4 getColor() { return m_color; }

	virtual void load(std::ifstream& file);
	virtual void save(std::ofstream& file);

	int id;

protected:

	int m_damage;

	std::string m_name;
	std::string m_description;

	GLuint m_texture;

	glm::ivec4 m_color;

};

class ItemSword : public Item {
public:
	ItemSword( int id, const std::string& name, const std::string& description, const std::string& texture, int damage, const glm::ivec4& color);
	~ItemSword() {}
};

class ItemPotion : public Item {
public:
	ItemPotion( int id, const std::string& name, const std::string& description, const std::string& texture, int healing, const glm::ivec4& color);
	~ItemPotion() {}

	int getDamage() { return -m_damage; }
	void onEquip();
	void onUse(Entity* entity);
};

class ItemFlask : public Item {
public:
	ItemFlask( int id, const std::string& name, const std::string& description, const std::string& texture, int healing, int uses, const glm::ivec4& color);
	~ItemFlask() {}

	int getDamage() { return -m_damage; }
	void onEquip();
	void onUse(Entity* entity);

	virtual void load(std::ifstream& file);
	virtual void save(std::ofstream& file);

protected:

	int m_uses;
};

namespace ItemSelector {

	static Item* getItem(int id){
		switch (id) {
		case -2:
			return new ItemSword(-2, "Dev Sword", "A developers hard hitting sword", "textures/PlayerSword.png", 20, glm::ivec4(255, 0, 255, 255));
		case -1:
			return new ItemFlask(-1, "Dev Flask", "A strong developers flask. Heals 5 HP", "textures/PotionHealth.png", 5, 9999, glm::ivec4(255, 0, 255, 255));
		case 0:
			return new ItemSword(0, "Basic Sword", "A basic sword", "textures/PlayerSword.png", 1, glm::ivec4(255, 255, 255, 255));
		case 1:
			return new ItemPotion(1, "Elven Potion", "A strong elven potion. Heals 5 HP", "textures/PotionHealth.png", 5, glm::ivec4(255));
		case 2:
			return new ItemPotion(2, "Lesser Elven Potion", "A mediocre elven potion. Heals 2 HP", "textures/PotionHealth.png", 2, glm::ivec4(200, 200, 200, 255));
		case 3:
			return new ItemPotion(3, "Elven Potion of Damage", "A powerful elven potion. Deals 4 damage", "textures/PotionHealth.png", -4, glm::ivec4(255, 128, 128, 255));
		case 4:
			return new ItemSword(4, "Goblin Sword", "A goblin's hard hitting sword", "textures/PlayerSword.png", 2, glm::ivec4(200, 255, 200, 255));
		case 5:
			return new ItemFlask(5, "Elven Flask", "A strong elven flask. Heals 5 HP", "textures/PotionHealth.png", 5, 5, glm::ivec4(255, 0, 255, 255));
		case 6:
			return new ItemFlask(6, "Goblin Flask", "A strong goblin flask. Deals 4 Damage. 5 uses", "textures/PotionHealth.png", -4, 5, glm::ivec4(255, 0, 0, 255));
		case 7:
			return new ItemPotion(7, "Trollish Potion", "A powerful trollish potion. Heals 6 HP", "textures/PotionHealth.png", 6, glm::ivec4(128, 255, 255, 255));
		case 8:
			return new ItemFlask(8, "Trollish Flask", "A powerful trollish flask. Heals 10 HP. 3 uses", "textures/PotionHealth.png", 10, 3, glm::ivec4(128, 255, 255, 255));
		default:
			return nullptr;
		}
	}
}

#endif