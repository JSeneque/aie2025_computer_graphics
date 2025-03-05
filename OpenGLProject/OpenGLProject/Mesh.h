#pragma once
#include <glm/glm.hpp>

struct Vertex {
	glm::vec4 position;
};

class Mesh
{
protected:
	unsigned int triCount;
	unsigned int vao, vbo;

public:
	Mesh() : triCount(0), vao(0), vbo(0) {};
	virtual ~Mesh();

	void Initialise();
	virtual void Draw();
};



