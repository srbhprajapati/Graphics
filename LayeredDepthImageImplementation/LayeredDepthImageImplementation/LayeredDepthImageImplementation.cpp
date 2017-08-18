// LayeredDepthImageImplementation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "GL\glew.h"
#include "GL\freeglut.h"

#include "WavefrontObjectIO.h"
#include "ShaderUtil.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

int WindowHandle = 0;

using namespace std;

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSwapBuffers();
	glutPostRedisplay();
}

void InitWindow(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitContextVersion(4, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	WindowHandle = glutCreateWindow("TestProject");


	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error : " << glewGetErrorString(err) << std::endl;
	}
	std::cout << "Status : Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

	
	if (WindowHandle < 1) {
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
			);
		exit(EXIT_FAILURE);
	}

	glutDisplayFunc(RenderFunction);
}


GLuint InitShader()
{
	ShaderUtil::ShaderProgramResult shaderProgram;

	ShaderUtil::ShaderSource vertexShader;
	
	vertexShader.shaderType = GL_VERTEX_SHADER;
	vertexShader.source += "#version 330				";
	vertexShader.source += "void main()					";
	vertexShader.source += "{							";
	vertexShader.source += "}							";

	ShaderUtil::ShaderSource fragmentShader;
	
	fragmentShader.shaderType = GL_FRAGMENT_SHADER;
	fragmentShader.source += "void main()				";
	fragmentShader.source += "{							";
	fragmentShader.source += "}							";
	
	std::vector<ShaderUtil::ShaderSource> allShaders;
	allShaders.push_back(vertexShader);
	allShaders.push_back(fragmentShader);

	if (!ShaderUtil::CreateShaderProgram(allShaders, shaderProgram))
	{
		std::cout << "ERROR: error in Creating Shader Program" << std::endl;
		std::cout << shaderProgram.debugString << std::endl;
		return 0;
	}

	return shaderProgram.programId;
}

void Initialize(int argc, char** argv)
{
	InitWindow(argc, argv);
	
	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
		);

	
	InitShader();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}



int _tmain(int argc, char* argv[])
{
	Initialize(argc, argv);

	std::vector<float> vertexArray;
	std::vector<unsigned int> indexArray;

	bool result = WavefrontObjectIO::LoadObjectFile("../datasets/Teapot.obj", &vertexArray, &indexArray);

	if (result)
	{
		std::cout << "File Opened successfully" << std::endl;
	}
	else
	{
		std::cout << "File not Opened successfully" << std::endl;
	}



	glutMainLoop();

	return 0;
}

