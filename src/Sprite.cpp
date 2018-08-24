/*

#include "Sprite.h"

Sprite::Sprite(){

}

Sprite::~Sprite(){

}

void Sprite::init(const glm::vec2& position, const glm::vec2& size, float rotation) {

	glGenVertexArrays(1, &m_vao);
	bind();

	setVBOData(position, size, rotation);

	unbind();
}

void Sprite::setVBOData(const glm::vec2& position, const glm::vec2& size, float rotation){
	this->m_position = position;
	this->m_size = size;
	this->m_rotation = rotation;

	std::vector<float> positions;
	std::vector<float> uvs;

	createTriangles(positions, uvs, position, size);

	addVBO(2, positions);
	addVBO(2, uvs);
}

void Sprite::bind(){
	glBindVertexArray(m_vao);
}

void Sprite::unbind(){
	glBindVertexArray(0);
}

void Sprite::createTriangles(std::vector<float>& positions,
			     std::vector<float>& uvs, 
			     const glm::vec2& position, const glm::vec2& size){
	positions.push_back(position.x);
	positions.push_back(position.y);
	uvs.push_back(0);
	uvs.push_back(0);

	positions.push_back(position.x + size.x);
	positions.push_back(position.y);
	uvs.push_back(1);
	uvs.push_back(0);
	
	positions.push_back(position.x);
	positions.push_back(position.y + size.y);
	uvs.push_back(0);
	uvs.push_back(1);

	positions.push_back(position.x + size.x);
	positions.push_back(position.y);
	uvs.push_back(1);
	uvs.push_back(0);
	
	positions.push_back(position.x);
	positions.push_back(position.y + size.y);
	uvs.push_back(0);
	uvs.push_back(1);

	positions.push_back(position.x + size.x);
	positions.push_back(position.y + size.y);
	uvs.push_back(1);
	uvs.push_back(1);
}

void Sprite::addVBO(int dimensions, const std::vector<float>& data){
	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(m_vbos.size());

	glVertexAttribPointer(m_vbos.size(), 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_vbos.push_back(vbo);
}

*/