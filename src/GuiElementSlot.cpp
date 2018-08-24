#include "GuiElementSlot.h"
#include "TextureCache.h"

#include "Base.h"
#include "Game.h"
#include "Player.h"

#include "GuiRenderer.h"

GuiElementSlot::GuiElementSlot(const glm::vec2 & position, const glm::vec2 & size, const glm::vec2 & posReal, const glm::vec2 & sizeReal, unsigned int slotId) : GuiElement(position, size, posReal, sizeReal, glm::ivec4(255), TextureCache::getTexture("textures/InventorySlot.png")), m_slot(slotId),
							   m_itemRender(position, size, posReal + glm::vec2(sizeReal / 10.f), sizeReal - glm::vec2(sizeReal/5.f), glm::ivec4(255), 1)
{
	
}

GuiElementSlot::~GuiElementSlot()
{
}

void GuiElementSlot::render()
{
	Player* player = Base::getGame()->getPlayer();
	if (player->getItem(m_slot) != nullptr) {
		m_itemRender.setTexture(player->getItem(m_slot)->getTexture());
		m_itemRender.color = player->getItem(m_slot)->getColor();
		GuiRenderer::addGUIElement(&m_itemRender);
	}
	GuiRenderer::addGUIElement(this);
}


