#ifndef FONTRENDERER_H
#define FONTRENDERER_H

#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>

#include <string>
#include <unordered_map>

#include "SpriteBatch.h"

class FontRenderer
{
public:

	struct Glyph {
		Glyph() : minx(0), maxx(0), miny(0), maxy(0), texture(0) {}
		~Glyph() {}
		int minx, maxx, miny, maxy, advance;
		int width, height;
		GLuint texture;
	};

	FontRenderer();
	~FontRenderer();

	void init(const std::string& path, unsigned int size);
	void destroy();

	void setText(const std::string& text, const glm::vec2& position, const glm::vec2& scale);

	void render();

private:
	TTF_Font* m_font;
	SpriteBatch m_batch;

	unsigned int m_size;

	std::unordered_map<char, Glyph> m_allGlyphs;
};

#endif

