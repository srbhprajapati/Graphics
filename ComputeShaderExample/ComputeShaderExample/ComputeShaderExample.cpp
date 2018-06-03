
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
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

using namespace std;

GLuint _ComputeShader=0;
GLuint _ShaderProgram = 0;

GLuint _FirstTexture = 0;
GLuint _SecondTexture = 0;

std::string GetComputeShaderSource()
{
	std::string src = "";


	src += "#version 430 																				\n";
	src += "layout (local_size_x = 16, local_size_y = 16, local_size_z=1) in;											\n";
	src += "																							\n";
	src += "layout(rgba32f) uniform image2D fromTex;															\n";
	src += "layout(rgba32f) uniform image2D toTex;															\n";
	src += "																							\n";
	src += "void main()																					\n";
	src += "{																							\n";
	src += "	ivec2 texCoord = ivec2(gl_GlobalInvocationID.xy);										\n";
	src += "	vec4 pixel = imageLoad(fromTex, texCoord);												\n";
	src += "	pixel.rg = pixel.gr;																	\n";
	src += "	imageStore(fromTex, texCoord, pixel);														\n";
	src += "	return;														\n";
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

void APIENTRY openglCallbackFunction(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{

	cout << "---------------------opengl-callback-start------------" << endl;
	cout << "message: " << message << endl;
	cout << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		cout << "OTHER";
		break;
	}
	cout << endl;

	cout << "id: " << id << endl;
	cout << "severity: ";
	switch (severity){
	case GL_DEBUG_SEVERITY_LOW:
		cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		cout << "HIGH";
		break;
	}
	cout << endl;

	cout << "---------------------opengl-callback-end--------------" << endl;
}


bool InitWindow(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitContextVersion(4, 3);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

	glutCreateWindow("TestProject");



	if (GLEW_OK != glewInit())
	{
		std::cout << "Error : glewinit Error" << std::endl;
	}

	if (glDebugMessageCallback){
		cout << "Register OpenGL debug callback " << endl;
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(openglCallbackFunction, nullptr);
		GLuint unusedIds = 0;
		glDebugMessageControl(GL_DONT_CARE,
			GL_DONT_CARE,
			GL_DONT_CARE,
			0,
			&unusedIds,
			true);
	}
	else
		cout << "glDebugMessageCallback not available" << endl;


	return InitComputeShader();
}

void DefineShaderVariables()
{
	
	glBindImageTexture(0, _FirstTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glBindImageTexture(1, _SecondTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);


	glUseProgram(_ShaderProgram);

	GLint image_location1 = glGetUniformLocation(_ShaderProgram, "fromTex");
	GLint image_location2 = glGetUniformLocation(_ShaderProgram, "toTex");

	glUniform1i(image_location1, 0);
	glUniform1i(image_location2, 1);

	glUseProgram(0);
}

bool CreateTexture()
{
	float* data = new float[4 * 16 * 16];

	for (int i = 0; i < 256; i++)
	{
		data[4 * i + 0] = 0;
		data[4 * i + 1] = 253;
		data[4 * i + 2] = 254;
		data[4 * i + 3] = 255;

	}

	_FirstTexture = GLUtilities::initTexture(16, 16, data);
	_SecondTexture = GLUtilities::initTexture(16, 16, nullptr);

	return (_FirstTexture != 0 && _SecondTexture != 0);
}

void ReadTexture(GLuint textureId, int height, int width)
{

	float* data = new float[4* 16 * 16];

	glBindTexture(GL_TEXTURE_2D, textureId);

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, data);

	glBindTexture(GL_TEXTURE_2D, 0);

	for (int i = 0; i < 16 * 16; i++)	
	{
		std::cout << i << " = " << data[i] << std::endl;
	}

}

void LaunchCompute()
{
	glUseProgram(_ShaderProgram);

	glDispatchCompute(16, 16, 1);

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

	ReadTexture(_FirstTexture, 16, 16);

	while (true){}

	return 0;
}

