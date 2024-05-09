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

	inline Vertex() : position(0.0f), normal(0.0f), texCoords(0.0f), tangent(0.0f), bitangent(0.0f) {}

	inline friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex)
	{ 
		return os << vertex.position << ' ' << vertex.normal << ' ' << vertex.texCoords << '\n';
	}
};
