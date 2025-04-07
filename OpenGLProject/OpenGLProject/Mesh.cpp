#include "Mesh.h"
#include "glad.h"
#include "assimp/scene.h"
#include "assimp/cimport.h"
#include "imgui_glfw3.h"

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void Mesh::InitialiseQuad()
{
	/*// check that the mesh is not initialised already
	assert(vao == 0);

	// generate buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// bind vertex array aka a mesh wrapper
	glBindVertexArray(vao);

	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);*/

	Vertex vertices[6];

	// define 6 vertices for 2 triangles
	vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };
	vertices[3].position = { 0.5f, 0, -0.5f, 1 };

	vertices[0].normal = { 0, 1, 0, 0 };
	vertices[1].normal = { 0, 1, 0, 0 };
	vertices[2].normal = { 0, 1, 0, 0 };
	vertices[3].normal = { 0, 1, 0, 0 };

	vertices[0].texCoord = { 0.0, 1.0 };
	vertices[1].texCoord = { 1.0, 1.0 };
	vertices[2].texCoord = { 0.0, 0.0 };
	vertices[3].texCoord = { 1.0, 0.0 };

	unsigned int indices[6] = { 0, 1, 2, 2, 1, 3 };

	Ka = glm::vec3(0.25, 0.25, 0);
	Kd = glm::vec3(0.5, 0.5, 0);
	Ks = glm::vec3(1);
	specularPower = 0.0f;

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
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// enable second element as normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	// enable third element as UVs
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

	// enable forth element as tangents
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)40);

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
		vertices[i].normal = glm::vec4(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 0);
		
		// TODO: Look into not making an assumption
		// does the array exists
		if (mesh->mTextureCoords[0])
		{
			vertices[i].texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, 1.0 - mesh->mTextureCoords[0][i].y);
		}
		else
		{
			vertices[i].texCoord = glm::vec2(0, 0);
		}

		if (mesh->HasTangentsAndBitangents())
		{
			vertices[i].tangent = glm::vec4{
				mesh->mTangents[i].x,
				mesh->mTangents[i].y,
				mesh->mTangents[i].z,
				1};
		}
		if (!mesh->HasTangentsAndBitangents())
			CalculateTangents(vertices, numV, indices);

	}

	Initialise(numV, vertices, indices.size(), indices.data());
	delete[] vertices;

}

void Mesh::Update(float dt)
{
	ImGui::Begin("Material Settings");
	ImGui::DragFloat3("Ambient", &Ka[0], 0.01f, 0.0f, 2.0f);
	ImGui::DragFloat3("Diffuse", &Kd[0], 0.01f, 0.0f, 2.0f);
	ImGui::DragFloat3("Specular", &Ks[0], 0.01f, 0.0f, 2.0f);
	ImGui::DragFloat("Power", &specularPower, 0.01f, 0.0f, 256.0f);
	ImGui::End();
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

void Mesh::ApplyMaterial(aie::ShaderProgram* shader)
{
	shader->bindUniform("Ka", Ka);
	shader->bindUniform("Kd", Kd);
	shader->bindUniform("Ks", Ks);
	shader->bindUniform("specularPower", specularPower);

	// bind the diffuse texture to 0
	mapKd.bind(0);
	shader->bindUniform("diffuseTex", 0);
	mapKs.bind(1);
	shader->bindUniform("specularTex", 1);
	mapBump.bind(2);
	shader->bindUniform("normalTex", 2);
}

void Mesh::LoadMaterial(const char* fileName)
{
	std::fstream file(fileName, std::ios::in);
	std::string line;
	std::string header;
	char buffer[256];

	// get the path part of the file name
	std::string directory(fileName);
	int index = directory.rfind('/');
	if (index != -1)
	{
		directory = directory.substr(0, index + 1);
	}
	
	while (!file.eof())
	{
		file.getline(buffer, 256);
		line = buffer;
		std::stringstream ss(line,
			std::stringstream::in | std::stringstream::out);
		if (line.find("Ka") == 0)
			ss >> header >> Ka.x >> Ka.y >> Ka.z;
		else if (line.find("Ks") == 0)
			ss >> header >> Ks.x >> Ks.y >> Ks.z;
		else if (line.find("Kd") == 0)
			ss >> header >> Kd.x >> Kd.y >> Kd.z;
		else if (line.find("Ns") == 0)
			ss >> header >> specularPower;
		else if (line.find("map_Kd") == 0)
		{
			std::string mapFileName;
			ss >> header >> mapFileName;
			mapKd.load((directory + mapFileName).c_str());
		}
		else if (line.find("map_Ks") == 0)
		{
			std::string mapFileName;
			ss >> header >> mapFileName;
			mapKs.load((directory + mapFileName).c_str());
		}
		else if (line.find("bump") == 0)
		{
			std::string mapFileName;
			ss >> header >> mapFileName;
			mapBump.load((directory + mapFileName).c_str());
		}
	}
}

void Mesh::LoadTexture(const char* fileName)
{
	mapKd.load(fileName);
}

void Mesh::CalculateTangents(Vertex* vertices, unsigned int vertexCount, const std::vector<unsigned int>& indices)
{
	glm::vec4* tan1 = new glm::vec4[vertexCount * 2];
	glm::vec4* tan2 = tan1 + vertexCount;
	memset(tan1, 0, vertexCount * sizeof(glm::vec4) * 2);
	unsigned int indexCount = (unsigned int)indices.size();
	for (unsigned int a = 0; a < indexCount; a += 3) {
		long i1 = indices[a];
		long i2 = indices[a + 1];
		long i3 = indices[a + 2];
		const glm::vec4& v1 = vertices[i1].position;
		const glm::vec4& v2 = vertices[i2].position;
		const glm::vec4& v3 = vertices[i3].position;
		const glm::vec2& w1 = vertices[i1].texCoord;
		const glm::vec2& w2 = vertices[i2].texCoord;
		const glm::vec2& w3 = vertices[i3].texCoord;
		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;
		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;
		float r = 1.0F / (s1 * t2 - s2 * t1);
		glm::vec4 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
		(t2 * z1 - t1 * z2) * r, 0);
		glm::vec4 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
		(s1 * z2 - s2 * z1) * r, 0);
		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;
		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
	}
	
	for (unsigned int a = 0; a < vertexCount; a++) {
		const glm::vec3& n = glm::vec3(vertices[a].normal);
		const glm::vec3& t = glm::vec3(tan1[a]);
		// Gram-Schmidt orthogonalize
		vertices[a].tangent = glm::vec4(glm::normalize(t - n * glm::dot(n,
		t)), 0);
		// Calculate handedness (direction of bitangent)
		vertices[a].tangent.w = (glm::dot(glm::cross(glm::vec3(n),
		glm::vec3(t)), glm::vec3(tan2[a])) < 0.0F) ? 1.0F : -1.0F;
	}
	delete[] tan1;
}


