#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "ShaderHandler.h"

class Camera {
public:
	Camera();
	~Camera();

	void init(const glm::vec2& position, float scale, float rotation);

	void update();
	/*
	 *Loads the camera matrix into the currently activated shader
	 * */
	void loadUniforms(ShaderHandler* shader);

	void setPosition(const glm::vec2& position) {m_position = position;
						     m_update = true;}
	void setScale(float scale) {m_scale = scale;
				    m_update = true;}
	void setRotation(float rotation) {m_rotation = rotation;
					  m_update = true;}

	glm::vec2 getPosition() {return m_position; }
	float getScale() {return m_scale; }
	float getRotation() {return m_rotation; }

private:
	glm::vec2 m_position;
	float m_scale;
	float m_rotation;

	glm::mat4 m_orthomatrix;
	glm::mat4 m_cameramatrix;
	
	bool m_update;

	/*Shader uniform location*/
	GLuint m_locMatrix;
};

#endif
