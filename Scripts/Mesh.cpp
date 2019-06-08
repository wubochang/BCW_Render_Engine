#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

//void Mesh::Initialize()
//{
//
//	m_vertices = std::vector<Vertex>
//	{
//		{glm::vec3(-0.5f, -0.5f, -0.5f),glm::vec4(1,1,1,1), glm::vec2(0.0f, 0.0f)},
//		{glm::vec3(0.5f, -0.5f, -0.5f),glm::vec4(1,1,1,1), glm::vec2(1.0f, 0.0f)},
//		{glm::vec3(0.5f, 0.5f, -0.5f),glm::vec4(1,1,1,1), glm::vec2(1.0f, 1.0f)},
//		{glm::vec3(0.5f, 0.5f, -0.5f),glm::vec4(1,1,1,1), glm::vec2(1.0f, 1.0f)},
//		{glm::vec3(-0.5f, 0.5f, -0.5f),glm::vec4(1,1,1,1), glm::vec2(0.0f, 1.0f)},
//		{glm::vec3(-0.5f, -0.5f, -0.5f),glm::vec4(1,1,1,1), glm::vec2(0.0f, 0.0f)},
//
//		{glm::vec3(-0.5f, -0.5f, 0.5f),glm::vec4(1,1,1,1), glm::vec2(0.0f, 0.0f)},
//		{glm::vec3(0.5f, -0.5f, 0.5f),glm::vec4(1,1,1,1), glm::vec2(1.0f, 0.0f)},
//		{glm::vec3(0.5f, 0.5f, 0.5f),glm::vec4(1,1,1,1), glm::vec2(1.0f, 1.0f)},
//		{glm::vec3(0.5f, 0.5f, 0.5f),glm::vec4(1,1,1,1), glm::vec2(1.0f, 1.0f)},
//		{glm::vec3(-0.5f, 0.5f, 0.5f),glm::vec4(1,1,1,1), glm::vec2(0.0f, 1.0f)},
//		{glm::vec3(-0.5f, -0.5f, 0.5f),glm::vec4(1,1,1,1), glm::vec2(0.0f, 0.0f)},
//
//		{glm::vec3(-0.5f, 0.5f, 0.5f),glm::vec4(1,1,1,1), glm::vec2(1.0f, 0.0f)},
//		{glm::vec3(-0.5f, 0.5f, -0.5f),glm::vec4(1,1,1,1), glm::vec2(1.0f, 1.0f)},
//		{glm::vec3(-0.5f, -0.5f, -0.5f),glm::vec4(1,1,1,1), glm::vec2(0.0f, 1.0f)},
//		{glm::vec3(-0.5f, -0.5f, -0.5f),glm::vec4(1,1,1,1), glm::vec2(0.0f, 1.0f)},
//		{glm::vec3(-0.5f, -0.5f, 0.5f),glm::vec4(1,1,1,1), glm::vec2(0.0f, 0.0f)},
//		{glm::vec3(-0.5f, 0.5f, 0.5f),glm::vec4(1,1,1,1), glm::vec2(1.0f, 0.0f)},
//
//		{glm::vec3(0.5f, 0.5f, 0.5f),glm::vec4(1,1,1,1), glm::vec2(1.0f, 0.0f)},
//		{glm::vec3(0.5f, 0.5f, -0.5f),glm::vec4(1,1,1,1), glm::vec2(1.0f, 1.0f)},
//		{glm::vec3(0.5f, -0.5f, -0.5f),glm::vec4(1,1,1,1), glm::vec2(0.0f, 1.0f)},
//		{glm::vec3(0.5f, -0.5f, -0.5f),glm::vec4(1,1,1,1), glm::vec2(0.0f, 1.0f)},
//		{glm::vec3(0.5f, -0.5f, 0.5f),glm::vec4(1,1,1,1), glm::vec2(0.0f, 0.0f)},
//		{glm::vec3(0.5f, 0.5f, 0.5f),glm::vec4(1,1,1,1), glm::vec2(1.0f, 0.0f)},
//
//		{glm::vec3(-0.5f, -0.5f, -0.5f),glm::vec4(1,1,1,1), glm::vec2(0.0f, 1.0f)},
//		{glm::vec3(0.5f, -0.5f, -0.5f),glm::vec4(1,1,1,1), glm::vec2(1.0f, 1.0f)},
//		{glm::vec3(0.5f, -0.5f, 0.5f),glm::vec4(1,1,1,1), glm::vec2(1.0f, 0.0f)},
//		{glm::vec3(0.5f, -0.5f, 0.5f),glm::vec4(1,1,1,1), glm::vec2(1.0f, 0.0f)},
//		{glm::vec3(-0.5f, -0.5f, 0.5f),glm::vec4(1,1,1,1), glm::vec2(0.0f, 0.0f)},
//		{glm::vec3(-0.5f, -0.5f, -0.5f),glm::vec4(1,1,1,1), glm::vec2(0.0f, 1.0f)},
//
//		{glm::vec3(-0.5f, 0.5f, -0.5f),glm::vec4(1,1,1,1), glm::vec2(0.0f, 1.0f)},
//		{glm::vec3(0.5f, 0.5f, -0.5f),glm::vec4(1,1,1,1), glm::vec2(1.0f, 1.0f)},
//		{glm::vec3(0.5f, 0.5f, 0.5f),glm::vec4(1,1,1,1), glm::vec2(1.0f, 0.0f)},
//		{glm::vec3(0.5f, 0.5f, 0.5f),glm::vec4(1,1,1,1), glm::vec2(1.0f, 0.0f)},
//		{glm::vec3(-0.5f, 0.5f, 0.5f),glm::vec4(1,1,1,1), glm::vec2(0.0f, 0.0f)},
//		{glm::vec3(-0.5f, 0.5f, -0.5f),glm::vec4(1,1,1,1), glm::vec2(0.0f, 1.0)}
//	};
//
//	m_indices = std::vector<unsigned int>();
//	for (unsigned int i = 0; i < m_vertices.size(); i++)
//	{
//		m_indices.push_back(i);
//	}
//
//	InitBuffers();
//}

void Mesh::InitBuffers()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	// bind VAO
	glBindVertexArray(m_VAO);

	// bind vertex array
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

	// bind index array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	// normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// vertex color
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// uv
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(10 * sizeof(float)));
	glEnableVertexAttribArray(3);

	// tangent
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(12 * sizeof(float)));
	glEnableVertexAttribArray(4);

	// unbind vertex array
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// unbind VAO
	glBindVertexArray(0);
}

void Mesh::Initialize(std::vector<Vertex> verts, std::vector<unsigned int> indices)
{
	m_vertices = verts;
	m_indices = indices;

	InitBuffers();
}

void Mesh::Shutdown()
{
}

void Mesh::SetBuffers()
{
	glBindVertexArray(m_VAO);
}

void Mesh::UnbindBuffers()
{
	glBindVertexArray(0);
}

