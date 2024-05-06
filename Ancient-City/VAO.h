#pragma once

#include "utils.h"
#include "vertex.h"

class VAO
{
public:
	VAO(const std::vector<Vertex>& vertices, const std::vector<uint>& indices);
	VAO(const VAO& other) noexcept = default;
	VAO(VAO&& other) noexcept : ID(other.ID) { other.ID = 0; }
	~VAO() noexcept;

	inline explicit operator int() { return ID; }
	void Bind() const;
	inline void Unbind() const { GLCall(glBindVertexArray(0)); }

private:
	uint ID;
};

