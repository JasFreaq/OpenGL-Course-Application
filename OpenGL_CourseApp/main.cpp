#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Window Properties
const GLint WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;
const char* WINDOW_TITLE = "Test Window";

int main()
{
	//Initialise GLFW
	if (!glfwInit())
	{
		printf("GLFW initialisation failed.");
		glfwTerminate();
		return 1;
	}

	//Setup GLFW Window Properties
	//OpenGL Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Disallow Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Allow Forwards Compatibility

	GLFWwindow* mainWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
	if (!mainWindow)
	{
		printf("GLFW window creation failed.");
		glfwTerminate();
		return 1;
	}

	//Get buffer size
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//Set GLEW context
	glfwMakeContextCurrent(mainWindow);

	//Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed.");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	//Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	while (!glfwWindowShouldClose(mainWindow))
	{
		//Handle User Inputs
		glfwPollEvents();

		//Clear Window
		glClearColor(1.f, 0.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}