#include "Game.h"
#include "Base.h"
#include "InputManager.h"
#include "TextureCache.h"
#include "TexturedShader.h"
#include "TileRenderer.h"
#include "FPSRegulator.h"
#include "GuiRenderer.h"

#include "TileRegistry.h"

#include <SDL2/SDL.h>

Game::Game(){

}

Game::~Game(){

}

void Game::init(){

	TileRegistry::init();

	TexturedShader::init();
	
	m_camera.init(glm::vec2(0), 1.f, 0.f);

	m_player.init(glm::uvec2(1, 1));
	m_level.init();

	GuiRenderer::init();

	m_hud.init();
}

void Game::destroy()
{
	TexturedShader::destroy();
	GuiRenderer::destroy();

	m_player.destroy();
	m_hud.destroy();
}

void Game::update(){

	if(InputManager::isKeyDown(SDLK_MINUS)) m_camera.setScale(m_camera.getScale() - 0.01f);
	if(InputManager::isKeyDown(SDLK_EQUALS)) m_camera.setScale(m_camera.getScale() + 0.01f);
	if (InputManager::isKeyPressed(SDLK_r)) TexturedShader::reload();

	m_player.update();

	m_camera.setPosition(glm::vec2(m_player.getPosition()) * glm::vec2(100.f) + glm::vec2(50));

	m_camera.update();

	m_level.update();

	m_hud.update();

	//0.185
	//0.3
}

void Game::render(){

	TexturedShader::begin();

	m_camera.loadUniforms(TexturedShader::getCore());

	glActiveTexture(GL_TEXTURE0);

	m_level.render();
	m_player.render();

	GuiRenderer::render(TexturedShader::getCore());

	m_hud.render();

	TexturedShader::end();

}

void Game::setScaleLimit(const glm::vec2 & scale)
{
	m_camera.setScaleLimit(scale);
	m_camera.setScale(scale.x);
}
