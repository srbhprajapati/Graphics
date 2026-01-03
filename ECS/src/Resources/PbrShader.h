#pragma once

#include "Shader.h"

class PbrShader : public Shader
{

public:

	PbrShader();


	void SetDiffuseMap(GLuint texId);
	void SetNormalMap(GLuint texId);
	void SetSpecularMap(GLuint texId);
	void SetEmissiveMap(GLuint texId);

private:


};