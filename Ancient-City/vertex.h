#pragma once

#include <iostream>
#include <glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;

	friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex)
	{
		os << vertex.position << ' ' << vertex.normal << ' ' << vertex.texCoords << '\n';
		return os;
	}
};
