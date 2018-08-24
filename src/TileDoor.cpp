#include "TileDoor.h"
#include "TextureCache.h"

#include "Base.h"
#include "Game.h"
#include "Level.h"

TileDoor::TileDoor(unsigned int id) : Tile(id, true, "textures/DoorLocked.png"){

}

TileDoor::~TileDoor() {

}

void TileDoor::collide()
{
	Level* level = Base::getGame()->getLevel();
	if (level->levelCleared()) {
		Player* player = Base::getGame()->getPlayer();

		glm::ivec2 plrpos = player->getPosition();

		level->enterDoorFrom(plrpos);
	}
	else {

	}
}

GLuint TileDoor::getTexture()
{
	if (Base::getGame()->getLevel()->levelCleared()) {
		return TextureCache::getTexture("textures/Door.png");
	}
	else {
		return TextureCache::getTexture("textures/DoorLocked.png");
	}
}

bool TileDoor::isSolid()
{
	if (Base::getGame()->getLevel()->levelCleared()) {
		return false;
	}
	else {
		return true;
	}
}
