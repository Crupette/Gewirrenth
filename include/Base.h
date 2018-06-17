#ifndef BASE_H
#define BASE_H

#include "Game.h"

#include <string>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Base {
public:
	static void run(const glm::uvec2& size, const std::string& name);
	static void init();

	static void gameLoop();
	static void update();
	static void render();
	static void readInputs();

	static glm::uvec2 getScreenSize() {return m_windowSize;}
	static void setWindowTitle(const std::string& title){
		SDL_SetWindowTitle(m_window, title.c_str());
	}

private:
	static bool m_quit;
	
	static Game m_game;
	
	static SDL_Window* m_window;
	static SDL_GLContext m_context;

	static glm::uvec2 m_windowSize;
};

#endif
