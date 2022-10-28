#pragma once

#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GLWindow
{

	const char* WINDOW_TITLE = "Test Window";

public:

	GLWindow();

	GLWindow(GLint windowWidth, GLint windowHeight);

	~GLWindow();

	int Initialise();

	GLint GetBufferWidth() const { return bufferWidth; }

	GLint GetBufferHeight() const { return bufferHeight; }

	bool ShouldCloseWindow() const { return glfwWindowShouldClose(mainWindow); }

	void SwapBuffers() const { glfwSwapBuffers(mainWindow); }

private:

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	GLFWwindow* mainWindow;

};

