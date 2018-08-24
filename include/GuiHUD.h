#ifndef GUIHUD_H
#define GUIHUD_H

#include "Gui.h"

#include "GuiElementBox.h"
#include "GuiElementSlot.h"
#include "FontRenderer.h"

class GuiHUD : public Gui
{
public:
	GuiHUD();
	~GuiHUD();

	void init();
	void destroy();

	void update();
	void render();

private:
	
	GuiElement* m_damageOverlay;
	GuiElement* m_healthBar;
	GuiElement* m_healthBackBar;

	GuiElementSlot* m_slotDisplay[10];

	FontRenderer m_fontRenderer;
	FontRenderer m_levelRender;
};

#endif