#include "CustomWindow.h"

CustomWindow::CustomWindow() : CustomWindow(1280, 720) { }

CustomWindow::CustomWindow(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}

	delX = 0.0f;
	delY = 0.0f;
}

CustomWindow::~CustomWindow()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

int CustomWindow::Initialise()
{
	//Initialise GLFW
	if (!glfwInit())
	{
		printf("GLFW initialisation failed.\n");
		glfwTerminate();
		return 1;
	}

	//Setup GLFW Window Properties
	//OpenGL Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Disallow Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Allow Forwards Compatibility

	mainWindow = glfwCreateWindow(width, height, WINDOW_TITLE, nullptr, nullptr);
	if (!mainWindow)
	{
		printf("GLFW window creation failed.\n");
		glfwTerminate();
		return 1;
	}

	//Get buffer size
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//Set GLEW context
	glfwMakeContextCurrent(mainWindow);

	CreateCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	//Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed.\n");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	//Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);

	return 0;
}

GLfloat CustomWindow::GetDelX()
{
	GLfloat del = delX;
	delX = 0.0f;

	return del;
}

GLfloat CustomWindow::GetDelY()
{
	GLfloat del = delY;
	delY = 0.0f;

	return del;
}

void CustomWindow::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	CustomWindow* customWindow = static_cast<CustomWindow*>(glfwGetWindowUserPointer(window));

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			if (key == GLFW_KEY_ESCAPE)
			{
				glfwSetWindowShouldClose(window, GL_TRUE);
				return;
			}

			customWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			customWindow->keys[key] = false;
		}
	}
}

void CustomWindow::HandleMouse(GLFWwindow* window, double xPos, double yPos)
{
	CustomWindow* customWindow = static_cast<CustomWindow*>(glfwGetWindowUserPointer(window));

	customWindow->lastX = xPos;
	customWindow->lastY = yPos;

	if (customWindow->hasMouseFirstMoved)
	{
		customWindow->delX = xPos - customWindow->lastX;
		customWindow->delY = customWindow->lastY - yPos;
	}
	else
	{
		customWindow->hasMouseFirstMoved = true;
	}	
}

void CustomWindow::CreateCallbacks()
{
	glfwSetKeyCallback(mainWindow, HandleKeys);
	glfwSetCursorPosCallback(mainWindow, HandleMouse);
}
