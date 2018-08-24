#ifndef GUIELEMENTSLOT_H
#define GUIELEMENTSLOT_H

#include "GuiElement.h"
#include "Item.h"

class GuiElementSlot : public GuiElement
{
public:
	GuiElementSlot(const glm::vec2& position, const glm::vec2& size, const glm::vec2& posReal, const glm::vec2& sizeReal, unsigned int slotId);
	~GuiElementSlot();

	void render();

private:

	GuiElement m_itemRender;
	unsigned int m_slot;
};

#endif