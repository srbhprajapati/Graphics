#include "GLFWWindow.h"
#include <iostream>
#include <string>



GLFWWindow::GLFWWindow(int width, int height, const std::string& title)
{
	if (!glfwInit())
	{
		isInitialized = false;
		return;
	}

	glfwSetErrorCallback(errorCallback);
	

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!window)
	{
		isInitialized = false;
		return;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glfwSwapInterval(1);
}

GLFWWindow::~GLFWWindow()
{
	if (isInitialized)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}

void GLFWWindow::PollEvents()
{

}

void GLFWWindow::SwapBuffers()
{
	glfwSwapBuffers(window);
}

int  GLFWWindow::GetWidth() const
{
	return -1;
}

int  GLFWWindow::GetHeight() const
{
	return -1;
}

bool GLFWWindow::ShouldClose() const 
{
	return false;
}

void* GLFWWindow::GetNativeHandle() const
{
	return nullptr;
}


void GLFWWindow::errorCallback(int error, const char* description)
{
	std::cout << "Error : " << description << std::endl;
}