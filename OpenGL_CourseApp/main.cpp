#include <cstdio>
#include <cstring>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Window Properties
const GLint WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;
const char* WINDOW_TITLE = "Test Window";

const float TO_RADIANS = 3.14159265358979323846f / 180.f;

GLuint vaoId, vboId, iboId, shaderId, uniformModel, uniformProjection;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float currentAngle = 0.0f;
float angleIncrement = 0.01f;

bool sizeDirection = true;
float currentSize = 0.5f;
float maxSize = 0.9f;
float minSize = 0.1f;
float sizeIncrement = 0.0002f;

//Shaders
static const char* vShader = R"(
#version 330

layout (location = 0) in vec3 pos;

out vec4 vColour;

uniform mat4 model;
uniform mat4 projection;

void main()
{
	gl_Position = projection * model * vec4(pos, 1.0f);
	vColour = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
})";

static const char* fShader = R"(
#version 330

in vec4 vColour;

out vec4 colour;

void main()
{
	colour = vColour;
})";

void CreateTriangle()
{
	unsigned int indices[] = {	0, 3, 1,
								1, 3, 2,
								2, 3, 0,
								0, 1, 2 };

	GLfloat vertices[] = { -1.f, -1.f, 0.f,
							0.f, -1.f, 1.f,
							1.f, -1.f, 0.f,
							0.f, 1.f, 0.f };

	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void AddShader(GLuint shaderProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	const GLchar* code[1];
	code[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(shader, 1, code, codeLength);
	glCompileShader(shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: %s\n", shaderType, eLog);
		return;
	}

	glAttachShader(shaderProgram, shader);
}

void CompileShaders()
{
	shaderId = glCreateProgram();

	if (!shaderId)
	{
		printf("Error creating shader program.\n");
		return;
	}

	AddShader(shaderId, vShader, GL_VERTEX_SHADER);
	AddShader(shaderId, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderId);
	glGetProgramiv(shaderId, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
		printf("Error linking program: %s\n", eLog);
		return;
	}

	glValidateProgram(shaderId);
	glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
		printf("Error validating program: %s\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shaderId, "model");
	uniformProjection = glGetUniformLocation(shaderId, "projection");
}

int main()
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

	GLFWwindow* mainWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
	if (!mainWindow)
	{
		printf("GLFW window creation failed.\n");
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
		printf("GLEW initialisation failed.\n");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	//Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	glm::mat4 projection = glm::perspective(45.0f, static_cast<GLfloat>(bufferWidth / bufferHeight), 0.1f, 100.0f);

	while (!glfwWindowShouldClose(mainWindow))
	{
		//Handle User Inputs
		glfwPollEvents();

		if (direction)
		{
			triOffset += triIncrement;
		}
		else
		{
			triOffset -= triIncrement;
		}
		if (abs(triOffset) > triMaxOffset)
			direction = !direction;

		currentAngle += angleIncrement;
		if (currentAngle >= 360.f)
			currentAngle -= 360.f;

		if (sizeDirection)
		{
			currentSize -= sizeIncrement;
		}
		else
		{
			currentSize += sizeIncrement;
		}
		if (currentSize > maxSize)
			sizeDirection = true;
		else if (currentSize < minSize)
			sizeDirection = false;

		//Clear Window
		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderId);

		glm::mat4 model(1.0f);
		model = translate(model, glm::vec3(0.f, 0.f, -2.5f));
		model = rotate(model, currentAngle * TO_RADIANS, glm::vec3(0.f, 1.f, 0.f));
		model = scale(model, glm::vec3(0.5f, 0.8f, 0.5f));

		glUniformMatrix4fv(uniformModel, 1.f, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1.f, GL_FALSE, glm::value_ptr(projection));
		
		glBindVertexArray(vaoId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}