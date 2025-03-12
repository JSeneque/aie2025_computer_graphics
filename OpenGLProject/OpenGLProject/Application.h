#pragma once
#include "glm/glm.hpp"
#include "glad.h"
#include "GLFW/glfw3.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"


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

	static Application* s_instance;
	glm::vec2 m_mousePosition;

	
	Camera camera;


public:
	bool Startup();
	bool Update(float dt);
	void Draw();
	void Shutdown();
	
	// singleton pattern
	static Application* get() { return  s_instance; }
	glm::vec2 GetMousePosition() { return m_mousePosition;  }
	void SetMousePosition(GLFWwindow* window, double x, double y);

};

