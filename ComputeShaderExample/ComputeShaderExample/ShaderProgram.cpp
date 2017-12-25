#include "stdafx.h"
#include "ShaderProgram.h"
#include <vector>
#include <iostream>
#include <fstream>

bool ShaderProgram::checkCompilationError(GLuint shader)
{
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> errInfo;
		errInfo.resize(maxLength);

		glGetShaderInfoLog(shader, maxLength, &maxLength, &errInfo[0]);

		//The program is useless now. So delete it.
		glDeleteShader(shader);

		//Print Error
		for (std::vector<GLchar>::const_iterator i = errInfo.begin(); i != errInfo.end(); ++i)
		{
			std::cout << *i;
		}
		std::cout << std::endl;

		return false;
	}

	return true;

}


bool ShaderProgram::checkLinkingError(GLuint shaderProgram)
{
	GLint isLinked = 0;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> errInfo;
		errInfo.resize(maxLength);

		glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &errInfo[0]);

		//The program is useless now. So delete it.
		glDeleteProgram(shaderProgram);

		//Print Error
		for (std::vector<GLchar>::const_iterator i = errInfo.begin(); i != errInfo.end(); ++i)
		{
			std::cout << *i;
		}
		std::cout << std::endl;

		return false;
	}

	return true;
}


//Yes
GLuint ShaderProgram::createShader(GLenum type, const GLchar* src)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	if (checkCompilationError(shader))
	{
		return shader;
	}
	else
	{
		return 0;
	}
}


GLuint ShaderProgram::createShader(GLenum type, const std::string filename)
{
	std::ifstream shaderFile;
	shaderFile.open(filename);

	std::string shaderContent;
	shaderContent.assign(std::istreambuf_iterator<char>(shaderFile), std::istreambuf_iterator<char>());

	const GLchar* src = shaderContent.c_str();
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	if (checkCompilationError(shader))
	{
		return shader;
	}
	else
	{
		return 0;
	}
}
