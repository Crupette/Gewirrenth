#ifndef GUIRENDERER_H
#define GUIRENDERER_H

#include "SpriteBatch.h"
#include "ShaderHandler.h"
#include "Camera.h"
#include "GuiElement.h"

class GuiRenderer
{
public:
	
	static void init();
	static void destroy();
	
	static void addGUIElement(GuiElement* element);

	static void render(ShaderHandler* shader);

private:
	
	static SpriteBatch m_batch;
	static Camera m_camera;

	static std::vector<GuiElement*> m_elements;
};

#endif