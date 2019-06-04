#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec4 color;
	glm::vec2 uv;
	glm::vec3 tangent;
};

#endif // !_VERTEX_H_
