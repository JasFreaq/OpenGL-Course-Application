#pragma once

#include <GL/glew.h>

class Mesh
{
public:

	Mesh();

	~Mesh();

	void CreateMesh(unsigned int* indices, GLfloat* vertices, unsigned int numIndices, unsigned int numVertices);

	void RenderMesh();

	void ClearMesh();

private:

	GLsizei indexCount;
	GLuint vaoId, vboId, iboId;

};