#pragma once

#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class CustomWindow
{

	const char* WINDOW_TITLE = "Test Window";

public:

	CustomWindow();

	CustomWindow(GLint windowWidth, GLint windowHeight);

	~CustomWindow();

	int Initialise();

	GLint GetBufferWidth() const { return bufferWidth; }

	GLint GetBufferHeight() const { return bufferHeight; }

	bool ShouldCloseWindow() const { return glfwWindowShouldClose(mainWindow); }

	bool* GetKeys() { return keys; }

	GLfloat GetDelX();

	GLfloat GetDelY();

	void SwapBuffers() const { glfwSwapBuffers(mainWindow); }

private:

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	GLFWwindow* mainWindow;

	bool keys[1024];

	bool hasMouseFirstMoved;
	GLfloat lastX, lastY, delX, delY;

	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);

	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);

	void CreateCallbacks();
};

