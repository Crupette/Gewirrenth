#include "Tile.h"
#include "TextureCache.h"

Tile::Tile(unsigned int id, bool solid, const std::string & texture) : id(id), m_solid(solid), m_texture(TextureCache::getTexture(texture))
{
}

Tile::~Tile()
{
}
