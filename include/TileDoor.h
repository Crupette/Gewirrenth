#ifndef TILEDOOR_H
#define TILEDOOR_H

#include "Tile.h"

class TileDoor : public Tile {
public:
	TileDoor(unsigned int id);
	~TileDoor();

	void collide();

	GLuint getTexture();

	bool isSolid();
};

#endif