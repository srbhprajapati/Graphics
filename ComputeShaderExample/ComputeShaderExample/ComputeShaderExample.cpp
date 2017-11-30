
// ComputeShaderExample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <GL\glew.h>

#include <gl/GL.h>
#include <gl\GLU.h>

#include <GL\freeglut.h>

#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800



void InitWindow(int argc, char** argv)
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
	//std::cout << "Status : Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
}


int main(int argc, char** argv)
{
	InitWindow(argc, argv);

	return 0;
}

