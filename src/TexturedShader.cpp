#include "TexturedShader.h"

ShaderHandler* TexturedShader::m_shader;

void TexturedShader::init()
{
	m_shader = new ShaderHandler();

	m_shader->addShader(GL_VERTEX_SHADER, "./shaders/textured.vert");
	m_shader->addShader(GL_FRAGMENT_SHADER, "./shaders/textured.frag");

	m_shader->addAttribute("position");
	m_shader->addAttribute("uv");

	m_shader->buildProgram();
}

void TexturedShader::reload()
{
	delete m_shader;
	init();
}

void TexturedShader::destroy()
{
	delete m_shader;
}

void TexturedShader::begin()
{
	m_shader->use();
}

void TexturedShader::end()
{
	m_shader->unuse();
}
