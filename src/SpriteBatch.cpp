#include "SpriteBatch.h"
#include "TextureCache.h"

#include <functional>
#include <algorithm>

Sprite::Sprite(const glm::vec2 & position, const glm::vec2 & size, const glm::ivec4& color, const std::string & texture) : position(position), size(size), color(color), texture(TextureCache::getTexture(texture)) {}

SpriteBatch::SpriteBatch()
{
}


SpriteBatch::~SpriteBatch()
{
}

void SpriteBatch::init()
{
	glGenVertexArrays(1, &m_vao);

	glBindVertexArray(m_vao);

	glGenBuffers(3, m_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteBatch::destroy()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(3, m_vbo);
}

void SpriteBatch::addSprite(const glm::vec4 & bounds, const glm::vec4 & uv, const glm::ivec4& color, int layer, GLuint texture)
{
	m_glyphs.emplace_back(bounds, uv, color, layer, texture);

	//printf("Recieved glyph with texture id %i\n", texture);
}

void SpriteBatch::addSprite(const Sprite sprite, int layer)
{
	m_glyphs.emplace_back(glm::vec4(sprite.position, sprite.size), glm::vec4(0.f, 0.f, 1.f, 1.f), sprite.color, layer, sprite.texture);
}

void SpriteBatch::begin()
{
	m_batches.clear();

	m_glyphs.clear();
	m_glyphptrs.clear();
}

void SpriteBatch::end()
{
	m_glyphptrs.resize(m_glyphs.size());
	for (size_t i = 0; i < m_glyphs.size(); i++) {
		m_glyphptrs[i] = &m_glyphs[i];
	}

	std::sort(m_glyphptrs.begin(), m_glyphptrs.end(), [](Glyph* a, Glyph* b) {return a->layer < b->layer; });

	createBatches();

	m_glyphs.clear();
	m_glyphptrs.clear();
}

void SpriteBatch::renderBatches()
{
	glBindVertexArray(m_vao);

	for (RenderBatch& batch : m_batches) {

		glBindTexture(GL_TEXTURE_2D, batch.texture);

		glDrawArrays(GL_TRIANGLES, batch.offset, batch.numVertices);

		//printf("VAO %i Rendering batch %p with %i vertices and %i offset. Texture: %i\n", m_vao, &batch, batch.numVertices, batch.offset, batch.texture);

		glBindTexture(GL_TEXTURE_2D, 0);

	}

	glBindVertexArray(0);
}

void SpriteBatch::createBatches()
{
	std::vector<float> positions;
	std::vector<float> uvs;
	std::vector<unsigned char> colors;

	if (m_glyphs.size() == 0) return;
	unsigned int offset = 0;

	m_batches.emplace_back(0, m_glyphptrs[0]->texture);
	positions.insert(positions.begin(), m_glyphptrs[0]->positions.begin(), m_glyphptrs[0]->positions.end());
	uvs.insert(uvs.begin(), m_glyphptrs[0]->uvs.begin(), m_glyphptrs[0]->uvs.end());
	colors.insert(colors.begin(), m_glyphptrs[0]->colors.begin(), m_glyphptrs[0]->colors.end());

	offset += 6;
	m_batches.back().numVertices += 6;


	for (size_t i = 1; i < m_glyphs.size(); i++) {
		if (m_batches.back().texture != m_glyphptrs[i]->texture) {
			m_batches.emplace_back(offset, m_glyphptrs[i]->texture);
		}
		positions.insert(positions.end(), m_glyphptrs[i]->positions.begin(), m_glyphptrs[i]->positions.end());
		uvs.insert(uvs.end(), m_glyphptrs[i]->uvs.begin(), m_glyphptrs[i]->uvs.end());
		colors.insert(colors.end(), m_glyphptrs[i]->colors.begin(), m_glyphptrs[i]->colors.end());

		offset += 6;
		m_batches.back().numVertices += 6;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(float), positions.data());

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);

	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(char), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, colors.size() * sizeof(char), colors.data());

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);

	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, uvs.size() * sizeof(float), uvs.data());
}