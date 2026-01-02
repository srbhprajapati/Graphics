#include "Shader.h"
#include <iostream>


Shader::Shader(std::string vertexShaderSource, std::string fragmentShaderSource)
{
	Compile(vertexShaderSource, fragmentShaderSource);
}

void Shader::Compile(std::string vertexShaderSource, std::string fragmentShaderSource)
{
	GLint success;

	const char* vSource = vertexShaderSource.c_str();
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vShader, 1, &vSource, nullptr);
	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(vShader, 512, nullptr, infoLog);
		std::cerr << "Vertex Shader compilation failed : " << infoLog << std::endl;
		return;
	}


	const char* fSource = fragmentShaderSource.c_str();
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fShader, 1, &fSource, nullptr);
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(fShader, 512, nullptr, infoLog);
		std::cerr << "Fragment Shader compilation failed : " << infoLog << std::endl;
		return;
	}


	shaderId = glCreateProgram();
	glAttachShader(shaderId, vShader);
	glAttachShader(shaderId, fShader);
	glLinkProgram(shaderId);

	glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shaderId, 512, nullptr, infoLog);
		std::cerr << "Program Linking Failed : " << infoLog << std::endl;

		return;
	}
	
	glDeleteShader(vShader);
	glDeleteShader(fShader);
}

void Shader::Bind() const 
{
	glUseProgram(shaderId);
}

void Shader::UnBind() const
{
	glUseProgram(0);
}

void Shader::SetUniformBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(shaderId, name.c_str()), (int)value);
}

void Shader::SetUniformInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(shaderId, name.c_str()), value);
}

void Shader::SetUniformFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(shaderId, name.c_str()), value);
}

void Shader::SetUniformVec3(const std::string& name, glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(shaderId, name.c_str()), 1, &value[0]);
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(shaderId, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
