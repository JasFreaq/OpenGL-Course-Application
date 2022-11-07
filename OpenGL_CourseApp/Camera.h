#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

using namespace glm;

class Camera
{
public:

	Camera();

	Camera(vec3 initPos, vec3 initWorldUp, GLfloat initYaw, GLfloat initPitch, GLfloat initMoveSpeed, GLfloat initTurnSpeed);

	~Camera();

	void KeyControl(bool* keys, GLfloat deltaTime);

	void MouseControl(GLfloat delX, GLfloat delY);

	mat4 CalculateViewMatrix();

private:

	vec3 position;
	vec3 forward, right, up;
	vec3 worldUp;

	GLfloat yaw, pitch;

	GLfloat moveSpeed, turnSpeed;

	void Update();
};

