
// ComputeShaderExample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <GL\glew.h>

#include <gl/GL.h>
#include <gl\GLU.h>

#include <GL\freeglut.h>

#include <iostream>
#include "ShaderProgram.h"
#include "GLUtilities.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

GLuint _ComputeShader=0;
GLuint _ShaderProgram = 0;

GLuint _FirstTexture = 0;
GLuint _SecondTexture = 0;

std::string GetComputeShaderSource()
{
	std::string src = "";

	src += "#version 430																				\n";
	src += "layout (local_size_x = 16, local_size_y = 16) in;											\n";
	src += "																							\n";
	src += "//layout(rgba32f, binding = 0) uniform readonly image2D fromTex;															\n";
	src += "//layout(rgba32f, binding = 1) uniform writeonly image2D toTex;															\n";
	src += "																							\n";
	src += "void main()																					\n";
	src += "{																							\n";
	src += "	//ivec2 texCoord = ivec2(gl_GlobalInvocationID.xy);										\n";
	src += "	//vec4 pixel = imageLoad(fromTex, texCoord);												\n";
	src += "	//pixel.rg = pixel.gr;																	\n";
	src += "	//imageStore(toTex, texCoord, pixel);														\n";
	src += "}																							\n";

	return src;
}

bool InitComputeShader()
{
	_ComputeShader = ShaderProgram::createShader(GL_COMPUTE_SHADER, GetComputeShaderSource().c_str());
	_ShaderProgram = glCreateProgram();

	if (_ComputeShader != 0 && _ShaderProgram != 0)
	{
		glAttachShader(_ShaderProgram, _ComputeShader);
		glLinkProgram(_ShaderProgram);

		if (!ShaderProgram::checkLinkingError(_ShaderProgram))
		{
			return false;
		}
		return true;
	}
	return false;
}


bool InitWindow(int argc, char** argv)
{
	glutInit(&argc, argv);
	//glutInitContextVersion(4, 0);
	//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("TestProject");


	if (GLEW_OK != glewInit())
	{
		//std::cout << "Error : " << glewGetErrorString(err) << std::endl;
	}

	return InitComputeShader();
}

void DefineShaderVariables()
{
	glUseProgram(_ShaderProgram);

	glBindImageTexture(0, _FirstTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(1, _SecondTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	glUseProgram(0);
}

bool CreateTexture()
{
	float* data = new float(16 * 16);

	for (int i = 0; i < 256; i++)
	{
		data[i] = i;
	}

	_FirstTexture = GLUtilities::initTexture(16, 16, data);
	_SecondTexture = GLUtilities::initTexture(16, 16, nullptr);

	return (_FirstTexture != 0 && _SecondTexture != 0);
}

void LaunchCompute()
{
	glUseProgram(_ShaderProgram);

//	glDispatchCompute(1, 1, 1);

	
	glUseProgram(0);
}

int main(int argc, char** argv)
{
	bool success = InitWindow(argc, argv);


	if (!success)
	{
		std::cout << "Error Creating Window" << std::endl;
	}

	if (!CreateTexture())
	{
		std::cout << "Error in creating new Textures" << std::endl;
	}

	DefineShaderVariables();

	LaunchCompute();

	return 0;
}

