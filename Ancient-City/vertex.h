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

	inline Vertex()
		: position(glm::vec3(0.0f)),
		normal(glm::vec3(0.0f)),
		texCoords(glm::vec2(0.0f)),
		tangent(glm::vec3(0.0f)),
		bitangent(glm::vec3(0.0f))
	{
		// empty
	}

	friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex)
	{
		os << vertex.position << ' ' << vertex.normal << ' ' << vertex.texCoords << '\n';
		return os;
	}
};
