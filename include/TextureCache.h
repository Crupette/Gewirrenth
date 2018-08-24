#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H

#include <GL/glew.h>
#include <string>
#include <vector>
#include <unordered_map>

class TextureCache
{
public:
	static GLuint getTexture(const std::string& file);
	static void getPixelDataFrom(const std::string& file, std::vector<unsigned char>& data, unsigned long& width, unsigned long& height);

	static void cleanUp();

private:

	static GLuint loadTexture(const std::string& file);
		
	static std::unordered_map<std::string, GLuint> m_textures;
};

#endif