#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"
#include <fstream>
#include <sstream>
#include <GLFW/glfw3native.h>
#include <vector>

struct Vertex {
	glm::vec4 position;
	glm::vec4 normal;
	glm::vec2 texCoord;
	glm::vec4 tangent;
};

namespace aie { class ShaderProgram; }

class Mesh
{
protected:
	unsigned int triCount;
	unsigned int vao, vbo, ibo;
	glm::vec3 Ka;			// ambient colour of the surface
	glm::vec3 Kd;			// diffuse colour of the surface
	glm::vec3 Ks;			// specular colour of the surface
	float specularPower;	// tightness of specular highlights

	aie::Texture mapKd;		// diffuse texture map
	aie::Texture mapKs;		// specular texture map
	aie::Texture mapBump;	// normal map

public:
	Mesh() : triCount(0), vao(0), vbo(0), ibo(0) {};
	virtual ~Mesh();

	void InitialiseQuad();
	void Initialise(unsigned int vertexCount, const Vertex* vertices
		, unsigned int indexCount = 0, unsigned int* indices = nullptr);

	void InitialiseFromFile(const char* fileName); 
	virtual void Update(float dt);
	virtual void Draw();

	void ApplyMaterial(aie::ShaderProgram* shader);
	void LoadMaterial(const char* fileName);
	void LoadTexture(const char* fileName);

	void CalculateTangents(Vertex* vertices, unsigned int vertexCount, const std::vector<unsigned int>& indices);
};



