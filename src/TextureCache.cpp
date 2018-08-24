#include "TextureCache.h"
#include "ErrorHandler.h"
#include "picoPNG.h"

#include <fstream>

std::unordered_map<std::string, GLuint> TextureCache::m_textures;

GLuint TextureCache::getTexture(const std::string & file)
{
	std::unordered_map<std::string, GLuint>::iterator it = m_textures.find(file);
	if (it == m_textures.end()) {
		GLuint texture = loadTexture(file);
		m_textures.emplace(file, texture);
		return texture;
	}
	else {
		return it->second;
	}
}

void TextureCache::getPixelDataFrom(const std::string & file, std::vector<unsigned char>& data, unsigned long& width, unsigned long& height)
{
	std::ifstream texfile(file, std::ios::binary);

	std::vector<char> fileData;

	size_t length = 0;

	if (texfile.fail()) {
		fatalError("Texture Cache: Invalid file", file, "... Loading blank texture");
	}

	texfile.seekg(0, std::ios::end);
	length = (size_t)texfile.tellg();
	texfile.seekg(0, std::ios::beg);

	fileData.resize(length);

	texfile.read(&fileData[0], length);

	decodePNG(data, width, height, (unsigned char*)(fileData.data()), length);
}

void TextureCache::cleanUp()
{
	for (auto& it : m_textures) {
		glDeleteTextures(1, &it.second);
	}
}

GLuint TextureCache::loadTexture(const std::string & file)
{
	GLuint textureId;

	unsigned long width = 0;
	unsigned long height = 0;

	std::vector<unsigned char> textureData;
	
	getPixelDataFrom(file, textureData, width, height);

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &textureData[0]);

	glBindTexture(GL_TEXTURE_2D, 0);

	return textureId;
}
