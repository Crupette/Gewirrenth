#ifndef SPRITE_H
#define SPRITE_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

struct Sprite {

	Sprite();
	~Sprite();

	void init(const glm::vec2& position, const glm::vec2& size, float rotation);
	void setVBOData(const glm::vec2& position, const glm::vec2& size, float rotation);

	void bind();
	void unbind();

	void setPosition(const glm::vec2& position) { setVBOData(position, m_size, m_rotation); }
	void setSize(const glm::vec2& size) { setVBOData(m_position, size, m_rotation); }
	void setRotation(float rotation) { setVBOData(m_position, m_size, rotation); }
	void setTranslations(const glm::vec2& position, const glm::vec2& size, float rotation) { setVBOData(position, size, rotation); }

	glm::vec2 getPosition() {return m_position;}
	glm::vec2 getSize() {return m_size; }
	float getRotation() {return m_rotation;}
private:

	void createTriangles(std::vector<float>& positions,
		   	     std::vector<float>& uvs, const glm::vec2& position,
			     const glm::vec2& size);

	void addVBO(int dimensions, const std::vector<float>& data);
	
	std::vector<GLuint> m_vbos;
	GLuint m_vao;

	glm::vec2 m_position;
	glm::vec2 m_size;
	float m_rotation;
};

#endif
