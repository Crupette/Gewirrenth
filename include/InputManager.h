#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <unordered_map>
#include <glm/glm.hpp>

class InputManager {
public:
	static void pressKey(unsigned int keyId);
	static void releaseKey(unsigned int keyId);

	static void moveMouse(unsigned int x, unsigned int y);
	static void scrollWheel(int direction);

	static void update();

	static bool isKeyDown(unsigned int keyId) {return m_keys[keyId];}

	static bool isKeyPressed(unsigned int keyId);

	static glm::uvec2 getMousePosition() { return m_mousePos; }
	static int getScrollWheelMovement() { return m_scrollWheel; }
private:

	static std::unordered_map<unsigned int, bool> m_keys;
	static std::unordered_map<unsigned int, bool> m_previous;
	
	static glm::uvec2 m_mousePos;
	static int m_scrollWheel;
};

#endif
