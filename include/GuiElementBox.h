#ifndef GUIELEMENTBOX_H
#define GUIELEMENTBOX_H

#include "GuiElement.h"
#include "TextureCache.h"

class GuiElementBox : public GuiElement
{
public:
	GuiElementBox(const glm::vec2& position, const glm::vec2& size, const glm::vec2& realPosition, const glm::vec2& realSize, const glm::ivec4& color) :
		GuiElement(position, size, realPosition, realSize, color, TextureCache::getTexture("textures/Blank.png"))
	{}
	~GuiElementBox() {}
};

#endif