#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Camera.h"
#include "Level.h"
#include "GuiHUD.h"
#include "FontRenderer.h"

class Game {
public:
	Game();
	~Game();

	void init();
	void destroy();

	void update();
	void render();

	Level* getLevel() { return &m_level; }
	Player* getPlayer() { return &m_player; }

	void setScaleLimit(const glm::vec2& scale);

private:
	Camera m_camera;
	Player m_player;

	Level m_level;

	GuiHUD m_hud;
};

#endif
