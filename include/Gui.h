#ifndef GUI_H
#define GUI_H

#include "GuiElement.h"

class Gui
{
public:
	Gui() {}
	~Gui() {}

	virtual void init() {}
	virtual void destroy() {}

	virtual void update() {}
	virtual void render() {}

private:
};

#endif