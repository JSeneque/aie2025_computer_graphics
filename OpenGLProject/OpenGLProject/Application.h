#pragma once
#include "glm/glm.hpp"
#include "glad.h"
#include "GLFW/glfw3.h"
#include "Mesh.h"
#include "Shader.h"


class Application
{
protected:
	const int windowWidth = 1280;
	const int windowHeight = 720;

	glm::mat4 view;
	glm::mat4 projection;

	GLFWwindow* window;

	Mesh mesh;

	aie::ShaderProgram shader;
	glm::mat4 quadTransform;

	float deltaTime;

public:
	bool Startup();
	bool Update(float dt);
	void Draw();
	void Shutdown();
};

