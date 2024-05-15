#pragma once
#include <glm/glm.hpp>

class Mesh
{
public:
	Mesh();
	~Mesh();

	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
	};

	void InitialiseQuad();
	void InitialiseFullscreenQuad();
	void Initialise(unsigned int _vertexCount, const Vertex* _vertices,
		unsigned int _indexCount = 0, unsigned int* _indeces = nullptr);

	void Draw();

private:
	unsigned int m_triCount;
	unsigned int m_vao; // The vertex object array
	unsigned int m_vbo; // the vertex buffer object
	unsigned int m_ibo; // the index buffer object

};
