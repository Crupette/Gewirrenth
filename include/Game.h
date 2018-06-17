#ifndef GAME_H
#define GAME_H

#include "ShaderHandler.h"
#include "Sprite.h"
#include "Camera.h"

class Game {
public:
	Game();
	~Game();

	void init();

	void update();
	void render();

private:
	ShaderHandler* m_shader;
	Sprite m_sprite;

	Camera m_camera;
};

#endif
