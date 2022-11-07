#include "Camera.h"

Camera::Camera() { }

Camera::Camera(vec3 initPos, vec3 initWorldUp, GLfloat initYaw, GLfloat initPitch, GLfloat initMoveSpeed,
               GLfloat initTurnSpeed)
{
	position = initPos;

	forward = vec3(0.0f, 0.0f, -1.0f);

	worldUp = initWorldUp;

	yaw = initYaw;
	pitch = initPitch;

	moveSpeed = initMoveSpeed;
	turnSpeed = initTurnSpeed;

	Update();
}

Camera::~Camera() { }

void Camera::KeyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += forward * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= forward * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
}

void Camera::MouseControl(GLfloat delX, GLfloat delY)
{
	delX *= turnSpeed;
	delY *= turnSpeed;

	yaw += delX;
	pitch = clamp(pitch + delY, -89.0f, 89.0f);

	Update();
}

mat4 Camera::CalculateViewMatrix()
{
	return lookAt(position, position + forward, worldUp);
}

void Camera::Update()
{
	forward.x = cos(radians(yaw)) * cos(radians(pitch));
	forward.y = sin(radians(pitch));
	forward.z = sin(radians(yaw)) * cos(radians(pitch));
	forward = normalize(forward);

	right = normalize(cross(forward, worldUp));
	up = normalize(cross(right, forward));
}
