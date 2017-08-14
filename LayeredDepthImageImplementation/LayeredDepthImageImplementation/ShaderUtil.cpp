#include "stdafx.h"
#include "ShaderUtil.h"
#include <iostream>
#include <fstream>

bool ShaderUtil::CreateShaderProgram(std::vector<ShaderUtil::ShaderSource> shaderSourceList, ShaderUtil::ShaderProgramResult& result)
{
	std::vector<GLuint> shaderIdList;
	if (!CreateAndValidateShaders(shaderSourceList, shaderIdList, result.debugString))
	{
		return false;
	}

	result.programId = glCreateProgram();
	for (int i = 0; i < shaderIdList.size(); i++)
	{
		glAttachShader(result.programId, shaderIdList[i]);
	}
	glLinkProgram(result.programId);

	return CheckLinkingError(result.programId, result.debugString);
}


bool ShaderUtil::CreateAndValidateShaders(std::vector<ShaderUtil::ShaderSource> shaderSourceList, std::vector<GLuint>& shaderIdList, std::string& debugString)
{
	if (!IsNecessaryShadersAvailable(shaderSourceList, debugString))
	{
		return false;
	}

	for (int i = 0; i < shaderSourceList.size(); i++)
	{
		GLuint shaderId;

		if (!CreateShader(shaderSourceList[i].shaderType, shaderSourceList[i].source.c_str(), shaderId, debugString))
		{
			return false;
		}

		shaderIdList.push_back(shaderId);
	}

	return true;
}

bool ShaderUtil::IsNecessaryShadersAvailable(std::vector<ShaderUtil::ShaderSource> shaderSourceList, std::string& debugString)
{
	bool isFragmentShaderAvailable = false, isVertexShaderAvailable = false;

	for (int i = 0; i < shaderSourceList.size(); i++)
	{
		if (shaderSourceList[i].shaderType == GL_VERTEX_SHADER || shaderSourceList[i].shaderType == GL_VERTEX_SHADER_ARB)
		{
			isVertexShaderAvailable = true;
		}
		else if (shaderSourceList[i].shaderType == GL_FRAGMENT_SHADER || shaderSourceList[i].shaderType == GL_FRAGMENT_SHADER_ARB)
		{
			isFragmentShaderAvailable = true;
		}
	}

	if (isFragmentShaderAvailable && isVertexShaderAvailable)
	{
		return true;
	}
	else
	{
		debugString += "One of the necessary Shader is unavailable \n";
		return false;
	}
}


bool ShaderUtil::CheckCompilationError(GLuint shader, std::string& debugString)
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
			debugString += *i;
		}

		debugString += "\n";

		return false;
	}

	return true;

}


bool ShaderUtil::CheckLinkingError(GLuint shaderProgram, std::string& debugString)
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
			debugString += *i;
		}

		debugString += "\n";
		return false;
	}
	
	return true;
}

bool ShaderUtil::CreateShader(GLenum type, const GLchar* src, GLuint& shaderId, std::string& debugString)
{
	shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, &src, nullptr);
	glCompileShader(shaderId);

	return CheckCompilationError(shaderId, debugString);
}


bool ShaderUtil::CreateShader(GLenum type, const std::string filename, GLuint& shaderId, std::string& debugString)
{
	std::ifstream shaderFile;
	shaderFile.open(filename);

	std::string shaderContent;
	shaderContent.assign(std::istreambuf_iterator<char>(shaderFile), std::istreambuf_iterator<char>());

	const GLchar* src = shaderContent.c_str();
	shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, &src, nullptr);
	glCompileShader(shaderId);

	return CheckCompilationError(shaderId, debugString);
}
