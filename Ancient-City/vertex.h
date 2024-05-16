#pragma once

#include <iostream>
#include <glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;

	inline Vertex() 
		: position(0.0f), normal(0.0f), texCoords(0.0f) {}
	inline Vertex(float x, float y, float z) 
		: position(x, y, z), normal(0.0f), texCoords(0.0f) {}

	inline friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex)
	{ 
		return os << vertex.position << ' ' << vertex.normal << ' ' << vertex.texCoords << '\n';
	}
};
