// LayeredDepthImageImplementation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "GL\glew.h"
#include "GL\freeglut.h"

#include "WavefrontObjectIO.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

int WindowHandle = 0;

using namespace std;

//void ResizeFunction(int Width, int Height)
//{
//  CurrentWidth = Width;
//  CurrentHeight = Height;
//  glViewport(0, 0, CurrentWidth, CurrentHeight);
//}

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


	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Glew Not Initialized" << std::endl;
	}

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	WindowHandle = glutCreateWindow("TestProject");

	if (WindowHandle < 1) {
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
			);
		exit(EXIT_FAILURE);
	}

	glutDisplayFunc(RenderFunction);
}


void Initialize(int argc, char** argv)
{
	InitWindow(argc, argv);

	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
		);

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

