#include "GuiRenderer.h"

SpriteBatch GuiRenderer::m_batch;
Camera GuiRenderer::m_camera;

std::vector<GuiElement*> GuiRenderer::m_elements;

void GuiRenderer::init()
{
	m_batch.init();

	m_camera.init(glm::vec2(0, 0), 1.f, 0.f);
	m_camera.update();
}

void GuiRenderer::destroy()
{
	m_batch.destroy();
}

void GuiRenderer::addGUIElement(GuiElement * element)
{
	m_elements.push_back(element);
}

void GuiRenderer::render(ShaderHandler* shader)
{
	m_camera.loadUniforms(shader);

	m_batch.begin();

	//printf("Rendering %i GUI elements\n", m_elements.size());

	for (GuiElement* element : m_elements) {
		//printf("Rendering element %p, with texture %i ... ", element, element->getTexture());
		m_batch.addSprite(element->convertToBounds(), glm::vec4(0, 0, 1, 1), element->color, 0, element->getTexture());
	}

	m_batch.end();

	m_batch.renderBatches();

	m_elements.clear();
}
