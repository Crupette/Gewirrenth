#include "GuiHUD.h"

#include "GuiRenderer.h"
#include "Base.h"
#include "Game.h"
#include "Player.h"

#include <sstream>

GuiHUD::GuiHUD()
{
}


GuiHUD::~GuiHUD()
{
}

void GuiHUD::init()
{

	m_healthBar = new GuiElementBox(glm::vec2(0.f, 0.f), glm::vec2(0, 0), glm::vec2(10, 10), glm::vec2(0, 20), glm::ivec4(255, 0, 0, 255));
	m_healthBackBar = new GuiElementBox(glm::vec2(0.f), glm::vec2(0.5f, 0.f), glm::vec2(10.f), glm::vec2(0.f, 20.f), glm::ivec4(64, 0, 0, 255));
	m_damageOverlay = new GuiElementBox(glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f), glm::vec2(0.f, 0.f), glm::vec2(0.f, 0.f), glm::ivec4(255, 0, 0, 64));

	for (int i = 0; i < 10; i++)
	{
		m_slotDisplay[i] = new GuiElementSlot(glm::vec2(0.f), glm::vec2(0.f, 0.f), glm::vec2((i * 50.f) + 262.f, 40.f), glm::vec2(50.f), i);
	}

	m_fontRenderer.init("font/Vera.ttf", 32);
	m_levelRender.init("font/Vera.ttf", 16);
}

void GuiHUD::destroy()
{
	delete m_healthBar;
	for (int i = 0; i < 10; i++) {
		delete m_slotDisplay[i];
	}
}

void GuiHUD::update()
{
	Player* player = Base::getGame()->getPlayer();
	m_healthBar->size = glm::vec2(((float)player->getHealth() / (float)player->getMaxHealth()) / 2.f, 0.f);
	GuiRenderer::addGUIElement(m_healthBackBar);
	GuiRenderer::addGUIElement(m_healthBar);

	for (int i = 0; i < 10; i++) {
		if (player->getSelectedSlot() == i) {
			m_slotDisplay[i]->setTexture(TextureCache::getTexture("textures/InventorySlotSelected.png"));
			if (m_slotDisplay[i]->sizeReal.x == 50.f) {
				m_slotDisplay[i]->posReal -= glm::vec2(5.f);
				m_slotDisplay[i]->sizeReal += glm::vec2(10.f);
				if (player->getSelectedItem() != nullptr) {
					m_fontRenderer.setText(player->getSelectedItem()->getName() + "\n" + player->getSelectedItem()->getDescription(), glm::vec2(262.f, 132.f), glm::vec2(0));
				}
				else {
					m_fontRenderer.setText("", glm::vec2(262.f, 100.f), glm::vec2(0));
				}
			}
		}
		else if (m_slotDisplay[i]->getTexture() == TextureCache::getTexture("textures/InventorySlotSelected.png")) {
			m_slotDisplay[i]->setTexture(TextureCache::getTexture("textures/InventorySlot.png"));
			if (m_slotDisplay[i]->size.x != 50.f) {
				m_slotDisplay[i]->posReal += glm::vec2(5.f);
				m_slotDisplay[i]->sizeReal -= glm::vec2(10.f);
			}
		}
		m_slotDisplay[i]->render();
	}

	if (player->isInvincible()) {
		GuiRenderer::addGUIElement(m_damageOverlay);
	}

	if (player->getHealth() == 0) {
		m_damageOverlay->color.r = 0;
		GuiRenderer::addGUIElement(m_damageOverlay);
		if (m_damageOverlay->color.a < 255) {
			m_damageOverlay->color.a++;
		}
	}

	static unsigned int prevId = 0;
	int level = 0;
	if (prevId != Base::getGame()->getLevel()->getLevelId()) {
		prevId = Base::getGame()->getLevel()->getLevelId();
		level = Base::getGame()->getLevel()->getLevel();

		std::stringstream ss;
		ss << "Room " << prevId << "\nDifficulty " << (int)floor(level / 5.f) << "\nLevel " << (int)level;

		m_levelRender.setText(ss.str(), glm::vec2(10.f, 742.f), glm::vec2(0));
	}
}

void GuiHUD::render()
{
	m_fontRenderer.render();
	m_levelRender.render();
}
