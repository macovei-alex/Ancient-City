#pragma once

#include "utils.h"
#include "vertex.h"

class VAO
{
public:
	VAO(const std::vector<Vertex>& vertices, const std::vector<uint>& indices);
	VAO(const VAO& other) = default;
	~VAO() { GLCall(glDeleteBuffers(GL_ARRAY_BUFFER, &ID)); }

	inline explicit operator int() { return ID; }
	inline void Bind() const { GLCall(glBindVertexArray(ID)); }
	inline void Unbind() const { GLCall(glBindVertexArray(0)); }

private:
	uint ID;
};

