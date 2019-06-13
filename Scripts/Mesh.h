
#ifndef _MESH_H_
#define _MESH_H_

#include "glad_include.h"

#include "Vertex.h"

#include <vector>

class Mesh
{
public:
	Mesh();
	~Mesh();

	void Initialize(std::vector<Vertex> verts, std::vector<unsigned int> indices);
	void Shutdown();
	void SetBuffers();
	void UnbindBuffers();

	int GetVertCnt()
	{
		return m_indices.size();
	}

	std::string name;

protected:

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;

	unsigned int m_VBO;
	unsigned int m_VAO;
	unsigned int m_EBO;

	void InitBuffers();
};

#endif // !_MESH_H_
