#include "UIMesh.h"

UIMesh::UIMesh()
{
}

UIMesh::~UIMesh()
{
}

void UIMesh::Initialize()
{
	m_vertices = std::vector<Vertex> 
	{
		// pos(vec3), normal(vec3), color(vec4), uv(vec2), tangent(vec3)
		{glm::vec3(-1.0f, -1.0f, 0.0f),	glm::vec3(0),	glm::vec4(1,1,1,1),		glm::vec2(0.0f, 0.0f),	glm::vec3(0)},
		{glm::vec3(-1.0f, 1.0f, 0.0f),	glm::vec3(0),	glm::vec4(1,1,1,1),		glm::vec2(0.0f, 1.0f),	glm::vec3(0)},
		{glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec3(0),	glm::vec4(1,1,1,1),		glm::vec2(1.0f, 1.0f),	glm::vec3(0)},
		{glm::vec3(1.0f, -1.0f, 0.0f),	glm::vec3(0),	glm::vec4(1,1,1,1),		glm::vec2(1.0f, 0.0f),	glm::vec3(0)}
	};

	m_indices = std::vector<unsigned int>
	{
		0, 1, 2,
		0, 2, 3
	};
	InitBuffers();
}
