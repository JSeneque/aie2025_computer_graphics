#pragma once
#include "glm/glm.hpp"
#include "glad.h"
#include "GLFW/glfw3.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

struct Light {
	glm::vec3 direction;
};

class Application
{
protected:
	const int windowWidth = 1280;
	const int windowHeight = 720;

	glm::mat4 view;
	glm::mat4 projection;

	GLFWwindow* window;

	Mesh dragonMesh;
	Mesh quadMesh;

	aie::ShaderProgram shader;
	aie::ShaderProgram m_phongShader;

	glm::mat4 dragonTransform;
	glm::mat4 quadTransform;

	float deltaTime;

	static Application* s_instance;

	glm::vec2 m_mousePosition;
	glm::vec2 m_lastMousePosition;
	
	Camera camera;

	struct Light {
		glm::vec3 direction;
		glm::vec3 colour;
	};

	Light directionalLight;
	glm::vec3 m_ambientLight;

	
public:
	//Application() {}
	bool Startup();
	bool Update(float dt);
	void Draw();
	void Shutdown();
	
	// singleton pattern
	static Application* Get();

	// mouse related functions
	glm::vec2 GetMousePosition() { return m_mousePosition;  }
	static void SetMousePosition(GLFWwindow* window, double x, double y);
	glm::vec2 GetMouseDelta()
	{
		return m_mousePosition - m_lastMousePosition;
	}

};

