#pragma once

#include <stdlib.h>
#include <GL\glew.h>
#include <GL\glut.h>
#include <string>

class ShaderProgram
{
	public:

		static bool checkCompilationError(GLuint shader);
		static bool checkLinkingError(GLuint shaderProgram);
		static GLuint createShader(GLenum type, const GLchar* src);
		static GLuint createShader(GLenum type, const std::string filename);

};

