#pragma once
#include "glm/glm.hpp"
#include "glad.h"
#include "GLFW/glfw3.h"

class Application
{
protected:
	const int windowWidth = 1280;
	const int windowHeight = 720;

	glm::mat4 view;
	glm::mat4 projection;

	GLFWwindow* window;

public:
	bool Startup();
	bool Update();
	void Draw();
	void Shutdown();
};

