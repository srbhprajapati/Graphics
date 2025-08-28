#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <GL/glew.h>

GLuint CreateShader(const GLchar* shaderCode, GLenum shaderType)
{
	GLuint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &shaderCode, NULL);
	glCompileShader(shaderId);

	GLint status;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::vector<GLchar> errorLog(infoLogLength);
		glGetShaderInfoLog(shaderId, infoLogLength, &infoLogLength, &errorLog[0]);

		std::string shaderName = "";
		if (shaderType == GL_VERTEX_SHADER) shaderName = "GL_VERTEX_SHADER";
		else if (shaderType == GL_FRAGMENT_SHADER) shaderName = "GL_FRAGMENT_SHADER";
		else if (shaderType == GL_GEOMETRY_SHADER) shaderName = "GL_GEOMETRY_SHADER";
		else if (shaderType == GL_COMPUTE_SHADER) shaderName = "GL_COMPUTE_SHADER";

		std::cout << "Error in compiling Shader. Shader Type : " << shaderType << " || ERROR MESSAGE : " << &errorLog[0] << std::endl;

		return 0;
	}

	return shaderId;
}

GLuint CreateProgram(std::vector<GLuint> shaders)
{
	GLuint programId = glCreateProgram();

	for (GLuint shader : shaders)
	{
		glAttachShader(programId, shader);
	}

	glLinkProgram(programId);

	GLint status;
	glGetProgramiv(programId, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength = 0;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::vector<GLchar> errorLog(infoLogLength);
		glGetProgramInfoLog(programId, infoLogLength, &infoLogLength, &errorLog[0]);

		std::cout << "Error in Linking Program. ERROR MESSAGE : " << &errorLog[0] << std::endl;

		return 0;
	}

	return programId;
}

