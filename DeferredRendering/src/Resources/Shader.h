#pragma once

#include <string>
#include "glad/glad.h"
#include <glm.hpp>



class Shader
{
public:
	
	Shader(std::string vertexShaderSource, std::string fragmentShaderSource);

	GLuint GetShaderId() { return shaderId; }


	void Bind();
	void UnBind();

	void SetUniformBool(const std::string& name, bool value) const;
	void SetUniformInt(const std::string& name, int value) const;
	void SetUniformFloat(const std::string& name, float value) const;
	void SetUniformVec3(const std::string& name, glm::vec3& value) const;
	void SetUniformMat4(const std::string& name, glm::mat4& value) const;


private:

	void Compile(std::string vertexShaderSource, std::string fragmentShaderSource);

	GLuint shaderId = 0;
};