#pragma once
#include <glm/glm.hpp>

struct Vertex {
	glm::vec4 position;
};

class Mesh
{
protected:
	unsigned int triCount;
	unsigned int vao, vbo, ibo;

public:
	Mesh() : triCount(0), vao(0), vbo(0), ibo(0) {};
	virtual ~Mesh();

	void InitialiseQuad();
	void Initialise(unsigned int vertexCount, const Vertex* vertices
		, unsigned int indexCount = 0, unsigned int* indices = nullptr);
	virtual void Draw();
};



