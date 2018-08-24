#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H

#include <vector>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

struct Sprite {
	Sprite(const glm::vec2& position, const glm::vec2& size, const glm::ivec4& color, const std::string& texture);
	~Sprite() {}

	glm::vec2 position;
	glm::vec2 size;
	glm::ivec4 color;

	GLuint texture;
};

class SpriteBatch
{
public:

	struct Glyph {
		Glyph(const glm::vec4& bounds, const glm::vec4& uv, const glm::ivec4& color, int layer, GLuint texture) : layer(layer), texture(texture){
			positions =
			{
				bounds.x, bounds.y,
				bounds.x + bounds.z, bounds.y,
				bounds.x, bounds.y + bounds.w,
				bounds.x + bounds.z, bounds.y,
				bounds.x, bounds.y + bounds.w,
				bounds.x + bounds.z, bounds.y + bounds.w
			};
			uvs = 
			{
				uv.x, uv.y,
				uv.x + uv.z, uv.y,
				uv.x, uv.y + uv.w,
				uv.x + uv.z, uv.y,
				uv.x, uv.y + uv.w,
				uv.x + uv.z, uv.y + uv.w
			};
			colors = {
				(unsigned char)color.r, (unsigned char)color.g, (unsigned char)color.b, (unsigned char)color.a,
				(unsigned char)color.r, (unsigned char)color.g, (unsigned char)color.b, (unsigned char)color.a,
				(unsigned char)color.r, (unsigned char)color.g, (unsigned char)color.b, (unsigned char)color.a,
				(unsigned char)color.r, (unsigned char)color.g, (unsigned char)color.b, (unsigned char)color.a,
				(unsigned char)color.r, (unsigned char)color.g, (unsigned char)color.b, (unsigned char)color.a,
				(unsigned char)color.r, (unsigned char)color.g, (unsigned char)color.b, (unsigned char)color.a,
			};
		}

		Glyph() {}

		int layer;
		GLuint texture;

		std::vector<float> positions;
		std::vector<float> uvs;
		std::vector<unsigned char> colors;
	};

	struct RenderBatch {
		RenderBatch(GLuint offset, GLuint texture) : offset(offset), texture(texture){}

		GLuint offset;
		GLuint texture;
		size_t numVertices = 0;
	};

	SpriteBatch();
	~SpriteBatch();

	void init();
	void destroy();

	void addSprite(const glm::vec4& bounds, const glm::vec4& uv, const glm::ivec4& color, int layer, GLuint texture);
	void addSprite(const Sprite sprite, int layer);

	void begin();
	void end();

	void renderBatches();

private:

	void createBatches();
	
	std::vector<Glyph> m_glyphs;
	std::vector<Glyph*> m_glyphptrs;

	std::vector<RenderBatch> m_batches;

	GLuint m_vao;
	GLuint m_vbo[3];
};

#endif