#include "InputManager.h"

std::unordered_map<unsigned int, bool> InputManager::m_keys;
std::unordered_map<unsigned int, bool> InputManager::m_previous;

glm::uvec2 InputManager::m_mousePos;
int InputManager::m_scrollWheel;

void InputManager::pressKey(unsigned int keyId){
	m_keys[keyId] = true;
}

void InputManager::releaseKey(unsigned int keyId){
	m_keys[keyId] = false;
}

void InputManager::moveMouse(unsigned int x, unsigned int y){
	m_mousePos.x = x;
	m_mousePos.y = y;	
}

void InputManager::scrollWheel(int direction)
{
	m_scrollWheel = direction;
}

void InputManager::update(){
	for(std::unordered_map<unsigned int, bool>::iterator it = m_keys.begin(); it != m_keys.end(); it++){
		m_previous[it->first] = it->second;
	}
	m_scrollWheel = 0;
}

bool InputManager::isKeyPressed(unsigned int keyId)
{
	auto it = m_keys.find(keyId);
	auto jt = m_previous.find(keyId);

	if (jt == m_previous.end() || it == m_keys.end()) return false;

	//printf("Key %i : SetCurrent %s, SetPrevious %s\n", keyId, it->second ? "true" : "false", jt->second ? "true" : "false");

	if (!it->second && jt->second) return true;
	return false;
}
