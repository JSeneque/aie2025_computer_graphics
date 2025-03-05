#include "Mesh.h"
#include "glad.h"
Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void Mesh::Initialise()
{
	// check that the mesh has not already initialised
	assert(vao == 0);
	// creates the vortex buffer and fill out vbo with its location on the graphics card
	glGenBuffers(1, &vbo);
	// create and fillout avo with the handle
	glGenVertexArrays(1, &vao);
	// bind vertex array to graphics card
	glBindVertexArray(vao);
	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// define 6 vertices for our 2 triangles
	Vertex vertices[6];

	// define 6 vertices for 2 triangles
	vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };

	vertices[3].position = { -0.5f, 0, -0.5f, 1 };
	vertices[4].position = { 0.5f, 0, 0.5f, 1 };
	vertices[5].position = { 0.5f, 0, -0.5f, 1 };

	// fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// enable the first element as position
	glEnableVertexAttribArray(0);
	// define it
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// quad has 2 triangles
	triCount = 2;
}

void Mesh::Draw()
{
	// tells opengl we want to use this vertex array object
	glBindVertexArray(vao);
	// we want to draw the elements as triangles
	glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);
}

// 1:22:27