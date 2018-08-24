#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include <glm/glm.hpp>
#include <GL/glew.h>

class GuiElement
{
public:
	GuiElement();
	GuiElement(const glm::vec2& position, const glm::vec2& size, const glm::vec2& realPosition, const glm::vec2& realSize, const glm::ivec4& color, GLuint texture);
	virtual ~GuiElement() {}

	virtual void update() {}
	virtual void render() {}

	glm::vec4 convertToBounds();

	GLuint getTexture() { return m_texture; }
	void setTexture(GLuint texture) { m_texture = texture; }

	glm::vec2 position;
	glm::vec2 size;

	glm::vec2 posReal;
	glm::vec2 sizeReal;

	glm::ivec4 color;

private:
	GLuint m_texture;
};

#endif