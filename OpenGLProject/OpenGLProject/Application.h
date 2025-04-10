#pragma once
#include "glm/glm.hpp"
#include "glad.h"
#include "GLFW/glfw3.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Instance.h"

struct PointLight
{
	glm::vec3 position;
	glm::vec3 color;
	float intensity;
};

class Application
{
protected:
	const int windowWidth = 1920;
	const int windowHeight = 1080;

	glm::mat4 view;
	glm::mat4 projection;

	GLFWwindow* window;

	Mesh soulspearMesh;
	//Mesh dragonMesh;
	//Mesh quadMesh;
	//Mesh deerMaskMesh;
	//Mesh winterValleyMesh;

	aie::ShaderProgram shader;
	aie::ShaderProgram m_phongShader;

	Instance* m_spearInstance;

	float deltaTime;

	static Application* s_instance;

	glm::vec2 m_mousePosition;
	glm::vec2 m_lastMousePosition;
	
	Camera* camera;

	Light directionalLight;
	glm::vec3 m_ambientLight;

	PointLight pointLight1;
	PointLight pointLight2;

	// --- Show ON/OFF Models
	bool showSoulspear = true;
	bool showPointLight1 = true;
	bool showPointLight2 = true;

	
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

