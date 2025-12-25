#pragma once

#include "Window.h"
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class GLFWWindow : public Window {

public:
    GLFWWindow(int width, int height, const std::string& title);
	~GLFWWindow();

    void PollEvents() override;
    void SwapBuffers() override;

    int GetWidth() const override;
    int GetHeight() const override;
    bool ShouldClose() const override;

    void* GetNativeHandle() const override;


    static void errorCallback(int error, const char* description);

private:

    bool isInitialized = false;

    GLFWwindow* window;
};