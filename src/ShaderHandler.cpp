#include "ShaderHandler.h"
#include "ErrorHandler.h"

#include <fstream>
#include <iostream>
#include <sstream>

ShaderHandler::ShaderHandler(){

}

ShaderHandler::~ShaderHandler(){
	glDeleteProgram(m_program);
}

void ShaderHandler::buildProgram(){

	m_program = glCreateProgram();
	std::vector<std::pair<GLuint, GLenum>> shaders;

	for(auto& it : tmp_shaders){
		shaders.push_back(compileShader(it.first, it.second));	
	}

	glLinkProgram(m_program);

	GLint linked = 0;
	glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
	if(linked == GL_FALSE){
		GLint logSize = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logSize);

		std::vector<GLchar> infoLog(logSize);
		glGetProgramInfoLog(m_program, logSize, &logSize, &infoLog[0]);

		glDeleteProgram(m_program);

		for(auto& it : shaders){
			glDeleteShader(it.first);
		}
	}

	for(auto& it : shaders){
		glDetachShader(m_program, it.first);
		glDeleteShader(it.first);
	}

	tmp_shaders.clear();
}


void ShaderHandler::addShader(GLenum type, const std::string& path){
	tmp_shaders.push_back(std::make_pair(type, path));
}

void ShaderHandler::addAttribute(const std::string& name){
	glBindAttribLocation(m_program, m_numAttributes++, name.c_str());
}

GLuint ShaderHandler::getUniformLocation(const std::string& name){
	return glGetUniformLocation(m_program, name.c_str());
}

void ShaderHandler::use(){
	glUseProgram(m_program);
}

void ShaderHandler::unuse(){
	glUseProgram(0);
}

std::pair<GLuint, GLenum> ShaderHandler::compileShader(GLenum type, const std::string& path){
	GLuint id = glCreateShader(type);

	std::ifstream file(path, std::ios::binary);
	if(file.fail()){
		fatalError("Shader program at ", this, " failed to compile: Shader ", path, " doesn't exist!");
	}



	std::string contents;
	GLint length;
	
	std::stringstream sstr;
	sstr << file.rdbuf();
	contents = sstr.str();

	length = contents.size();

	file.close();

	const char* data = contents.c_str();
	glShaderSource(id, 1, &(data), &length);

	glCompileShader(id);
	handleErrors(id);

	glAttachShader(m_program, id);

	return std::make_pair(id, type);
}

bool ShaderHandler::handleErrors(GLuint id){
	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	
	if(success == GL_FALSE){

		GLint logSize = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logSize);

		std::vector<GLchar> errorLog(logSize);
		glGetShaderInfoLog(id, logSize, &logSize, &errorLog[0]);

		glDeleteShader(id);

		fatalError("Shader program at ", this, " failed to compile: Shader id ", id, " failed to compile.\n Error log:\n", std::string(errorLog.data()));

		return false;
	}

	return true;
}
