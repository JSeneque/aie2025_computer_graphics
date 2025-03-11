#include "Mesh.h"
#include "glad.h"
#include "assimp/scene.h"
#include "assimp/cimport.h"
#include <vector>


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void Mesh::InitialiseQuad()
{
	//Vertex vertices[6];

	//// define 6 vertices for 2 triangles
	//vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	//vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	//vertices[2].position = { -0.5f, 0, -0.5f, 1 };

	//vertices[3].position = { -0.5f, 0, -0.5f, 1 };
	//vertices[4].position = { 0.5f, 0, 0.5f, 1 };
	//vertices[5].position = { 0.5f, 0, -0.5f, 1 };

	//Initialise(6, vertices);


	// with IBO
	Vertex vertices[4];

	// define 6 vertices for 2 triangles
	vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };
	vertices[3].position = { 0.5f, 0, -0.5f, 1 };

	unsigned int indices[6] = { 0, 1, 2, 2, 1, 3 };

	Initialise(4, vertices, 6, indices);

}

void Mesh::Initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount, unsigned int* indices)
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
	// fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	// enable the first element as position
	glEnableVertexAttribArray(0);
	// define it
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	
	// bind indices if there are any
	if (indexCount != 0) {
		glGenBuffers(1, &ibo);
		// bind
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		// fill it
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		triCount = indexCount / 3;
	}
	else {
		triCount = vertexCount / 3;
	}

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}

void Mesh::InitialiseFromFile(const char* fileName)
{
	// read vertices from the model
	const aiScene* scene = aiImportFile(fileName, 0);
	// use the first mesh
	aiMesh* mesh = scene->mMeshes[0];
	// extract the indices from the mesh
	int numFaces = mesh->mNumFaces;
	std::vector<unsigned int> indices;

	for (int i = 0; i < numFaces; ++i)
	{
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);

		// generate a second triangle if face is a quad
		if (mesh->mFaces[i].mNumIndices == 4)
		{
			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[3]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}
	}

	// generate vertices data
	int numV = mesh->mNumVertices;
	Vertex* vertices = new Vertex[numV];
	for (int i = 0; i < numV; ++i) 
	{
		vertices[i].position = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1);
		//TODO: UVS, NORMALS


	}

	Initialise(numV, vertices, indices.size(), indices.data());
	delete[] vertices;

}

void Mesh::Draw()
{
	// tells opengl we want to use this vertex array object
	glBindVertexArray(vao);
	
	if (ibo != 0) {
		glDrawElements(GL_TRIANGLES, 3 * triCount, GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);
	}
	
	// we want to draw the elements as triangles
	
}

// 1:22:27