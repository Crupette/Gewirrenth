#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H

#include <GL/glew.h>
#include <string>
#include <vector>

class ShaderHandler {
public:
	ShaderHandler();
	~ShaderHandler();

	void buildProgram();
	void linkShaders();

	void addShader(GLenum type, const std::string& path);

	void addAttribute(const std::string& name);

	GLuint getUniformLocation(const std::string& name);

	void use();
	void unuse();

private:

	std::pair<GLuint, GLenum> compileShader(GLenum type, const std::string& path);
	bool handleErrors(GLuint id);

	std::vector<std::pair<GLenum, std::string>> tmp_shaders;

	GLuint m_program = 0;
	GLuint m_numAttributes = 0;
};

#endif
