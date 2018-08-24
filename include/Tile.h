#ifndef TILE_H
#define TILE_H

#include <string>

#include <glm/glm.hpp>
#include <GL/glew.h>

class Tile
{
public:
	Tile(unsigned int id, bool solid, const std::string& texture);
	virtual ~Tile();

	virtual void collide() {}
	virtual void levelCleared() {}

	virtual GLuint getTexture() { return m_texture; }
	virtual bool isSolid() { return m_solid; }

	unsigned int id;

protected:
	GLuint m_texture;
	bool m_solid;
};

#endif
