#include "InputManager.h"

std::unordered_map<unsigned int, bool> InputManager::m_keys;
std::unordered_map<unsigned int, bool> InputManager::m_previous;

glm::uvec2 InputManager::m_mousePos;

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

void InputManager::update(){
	for(std::unordered_map<unsigned int, bool>::iterator it = m_keys.begin(); it != m_keys.end(); it++){
		m_previous[it->first] = it->second;
	}
}
