#include "Base.h"
#include "ErrorHandler.h"
#include "InputManager.h"

bool Base::m_quit;

Game Base::m_game;

SDL_Window* Base::m_window;
SDL_GLContext Base::m_context;

glm::uvec2 Base::m_windowSize;

void Base::run(const glm::uvec2& size, const std::string& name){
	m_windowSize = size;
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	m_window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowSize.x, m_windowSize.y, SDL_WINDOW_OPENGL);
	
	if(m_window == nullptr){
		fatalError("Failed to initialize window! Error", 1);
	}

	m_context = SDL_GL_CreateContext(m_window);

	glewExperimental = true;
	
	GLenum result = glewInit();
	if(result != GLEW_OK){
		fatalError("Failed to initialize GLEW! Returned |", glewGetErrorString(result), "Error", 2);
	}
	
	init();
	gameLoop();

	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Base::init(){
	glClearColor(0.4f, 0.4f, 1.f, 1.f);
	m_game.init();
}

void Base::gameLoop(){
	while(!m_quit){
		readInputs();
		update();
		render();
	}
}

void Base::update(){
	m_game.update();
}

void Base::render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_game.render();

	SDL_GL_SwapWindow(m_window);
}

void Base::readInputs(){
	InputManager::update();
	SDL_Event evnt;
	while(SDL_PollEvent(&evnt)){
		switch(evnt.type){
			case SDL_KEYDOWN:
				InputManager::pressKey(evnt.key.keysym.sym);
			break;
			case SDL_KEYUP:
				InputManager::releaseKey(evnt.key.keysym.sym);
			break;
			case SDL_MOUSEMOTION:
				InputManager::moveMouse(evnt.motion.x, evnt.motion.y);
			break;
			case SDL_QUIT:
				m_quit = true;
			break;
		}
	}
}
