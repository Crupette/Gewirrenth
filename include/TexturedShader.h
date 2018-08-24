#ifndef TEXTUREDSHADER_H
#define TEXTUREDSHADER_H

#include "ShaderHandler.h"

class TexturedShader
{
public:

	static void init();
	static void reload();

	static void destroy();

	static void begin();
	static void end();

	static ShaderHandler* getCore() { return m_shader; }

private:
	static ShaderHandler* m_shader;
};

#endif