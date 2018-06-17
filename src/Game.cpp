#include "Game.h"
#include "InputManager.h"

#include <SDL2/SDL.h>

Game::Game() : m_shader(nullptr){

}

Game::~Game(){

}

void Game::init(){

	printf("First part\n");

	m_shader = new ShaderHandler();

	printf("Created shader object %p\n", m_shader);

	m_shader->addShader(GL_VERTEX_SHADER, "./shaders/simple.vert");
	m_shader->addShader(GL_FRAGMENT_SHADER, "./shaders/simple.frag");

	printf("Added shader specifications\n");

	m_shader->addAttribute("position");
	m_shader->addAttribute("uv");

	printf("Added attributes\n");

	m_shader->buildProgram();

	printf("Finished initializing\n");

	m_sprite.init(glm::vec2(-50, -50), glm::vec2(100, 100), 0.f);
	
	m_camera.init(glm::vec2(0), 1.f, 0.f);
}

void Game::update(){
	m_camera.update();

	if(InputManager::isKeyPressed(SDLK_w)) m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.f, 1.f));
	if(InputManager::isKeyPressed(SDLK_s)) m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.f, -1.f));
	if(InputManager::isKeyPressed(SDLK_a)) m_camera.setPosition(m_camera.getPosition() + glm::vec2(-1.f, 0.f));
	if(InputManager::isKeyPressed(SDLK_d)) m_camera.setPosition(m_camera.getPosition() + glm::vec2(1.f, 0.f));
}

void Game::render(){

	m_shader->use();

	m_camera.loadUniforms(m_shader);

	m_sprite.bind();

	glDrawArrays(GL_TRIANGLES, 0, 6);

	m_sprite.unbind();

	m_shader->unuse();
}
