#include "Camera.h"
#include "Base.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() : m_position(0), m_scale(1.f), m_rotation(0), m_orthomatrix(1), m_cameramatrix(1), m_update(true), m_locMatrix(0), m_scaleLimit(0.f, 1.f){

}

Camera::~Camera(){

}

void Camera::init(const glm::vec2& position, float scale, float rotation){
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;

	m_update = true;
}

void Camera::update(){
	if(m_update){
		glm::uvec2 screendims = Base::getScreenSize();
		m_orthomatrix = glm::ortho(-(float)(screendims.x/2), (float)(screendims.x/2), -(float)(screendims.y/2), (float)(screendims.y/2));
		glm::vec3 translate(-m_position.x, -m_position.y, 0.f);
		m_cameramatrix = glm::scale(m_orthomatrix, glm::vec3(m_scale, m_scale, 0.f));
		m_cameramatrix = glm::translate(m_cameramatrix, translate);

		m_update = false;
	}
}

void Camera::loadUniforms(ShaderHandler* shader){
	if(m_locMatrix == 0){
		m_locMatrix = shader->getUniformLocation("cameraMatrix");	
	}

	glUniformMatrix4fv(m_locMatrix, 1, GL_FALSE, &(m_cameramatrix[0][0]));
}

void Camera::setScale(float scale)
{
	m_scale = scale;
	if (m_scale > m_scaleLimit.y) m_scale = m_scaleLimit.y;
	if (m_scale < m_scaleLimit.x) m_scale = m_scaleLimit.x;
	m_update = true;
}
