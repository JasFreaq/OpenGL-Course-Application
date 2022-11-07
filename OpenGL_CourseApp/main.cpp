#include <cstring>
#include <cmath>
#include <vector>

using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

#include "CustomWindow.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

vector<Mesh*> meshes;
vector<Shader*> shaders;
Camera camera;

//Shaders
static const char* vShaderFilePath = "Shaders/shader.vert";
static const char* fShaderFilePath = "Shaders/shader.frag";

void CreatePyramid()
{
	unsigned int indices[] = {	0, 3, 1,
								1, 3, 2,
								2, 3, 0,
								0, 1, 2 };

	GLfloat vertices[] = { -1.f, -1.f, 0.f,
							0.f, -1.f, 1.f,
							1.f, -1.f, 0.f,
							0.f, 1.f, 0.f };

	Mesh* mesh1 = new Mesh();
	mesh1->CreateMesh(indices, vertices, 12, 12);
	meshes.push_back(mesh1);
}

void CreateShader()
{
	Shader* shader = new Shader();
	shader->CreateFromFile(vShaderFilePath, fShaderFilePath);
	shaders.push_back(shader);
}

int main()
{
	CustomWindow window = CustomWindow();
	window.Initialise();

	CreatePyramid();
	CreateShader();

	camera = Camera(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f),
		-90.0f, 0.0f, 5.0f, 0.05f);

	GLfloat aspect = static_cast<GLfloat>(window.GetBufferWidth() / window.GetBufferHeight());
	mat4 projection = perspective(45.0f, aspect, 0.1f, 100.0f);

	while (!window.ShouldCloseWindow())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		//Handle User Inputs
		glfwPollEvents();

		camera.KeyControl(window.GetKeys(), deltaTime);
		camera.MouseControl(window.GetDelX(), window.GetDelY());

		//Clear Window
		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaders[0]->UseShader();

		mat4 model(1.0f);
		model = translate(model, vec3(0.f, 0.f, -2.5f));
		model = rotate(model, radians(120.f), vec3(0.f, 1.f, 0.f));
		model = scale(model, vec3(0.5f, 0.8f, 0.5f));

		glUniformMatrix4fv(shaders[0]->GetModelLocation(), 1.f, GL_FALSE, value_ptr(model));
		glUniformMatrix4fv(shaders[0]->GetViewLocation(), 1.f, GL_FALSE, value_ptr(camera.CalculateViewMatrix()));
		glUniformMatrix4fv(shaders[0]->GetProjectionLocation(), 1.f, GL_FALSE, value_ptr(projection));
		
		meshes[0]->RenderMesh();

		glUseProgram(0);

		window.SwapBuffers();
	}

	return 0;
}