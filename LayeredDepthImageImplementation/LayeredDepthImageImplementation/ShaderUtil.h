#pragma once

#include <string>
#include <GL/glew.h>
#include <vector>

class ShaderUtil
{
public:

	struct ShaderSource
	{
		std::string source;
		GLenum shaderType;
	};

	struct ShaderProgramResult
	{
		std::string debugString;
		GLuint programId;
	};

	static bool CreateShaderProgram(std::vector<ShaderSource> shaderSourceList, ShaderProgramResult& result);

private:

	static bool CreateAndValidateShaders(std::vector<ShaderSource> shaderSourceList, std::vector<GLuint>& shaderIdList, std::string& debugString);
	static bool CreateShader(GLenum type, const GLchar* src, GLuint& shaderId, std::string& debugString);
	static bool CreateShader(GLenum type, const std::string filename, GLuint& shaderId, std::string& debugString);

	static bool IsNecessaryShadersAvailable(std::vector<ShaderSource> shaderSourceList, std::string& debugString);
	static bool CheckCompilationError(GLuint shader, std::string& debugString);
	static bool CheckLinkingError(GLuint shaderProgram, std::string& debugString);

};

