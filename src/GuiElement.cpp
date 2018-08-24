#include "GuiElement.h"

#include "TextureCache.h"
#include "Base.h"

GuiElement::GuiElement() :
	position(0, 0), size(0, 0), posReal(0, 0), sizeReal(0, 0), color(255, 255, 255, 255), m_texture(TextureCache::getTexture("textures/Blank.png"))
{
}

GuiElement::GuiElement(const glm::vec2 & position, const glm::vec2 & size, const glm::vec2 & realPosition, const glm::vec2 & realSize, const glm::ivec4& color, GLuint texture) :
	position(position), size(size), posReal(realPosition), sizeReal(realSize), color(color), m_texture(texture)
{
}

glm::vec4 GuiElement::convertToBounds()
{
	glm::vec2 scaledPosition = position * glm::vec2(Base::getScreenSize());
	glm::vec2 scaledSize = size * glm::vec2(Base::getScreenSize());
	scaledPosition -= glm::vec2(Base::getScreenSize()) / 2.f;
	return glm::vec4((scaledPosition + posReal), (scaledSize + sizeReal));
}
