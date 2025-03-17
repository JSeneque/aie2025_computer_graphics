#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Application;

class Camera
{
private:
	float theta;
	float phi;
	glm::vec3 m_position;
	float turnSpeed;

public:
	Camera() {}
	Camera(glm::vec3 initialPos) : m_position(initialPos), theta(0), phi(0), turnSpeed(0.1f) {};

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float w, float h);
	void Update(float dt, GLFWwindow* window);
};

