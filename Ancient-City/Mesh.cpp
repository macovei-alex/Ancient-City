#include "Mesh.h"

#include <vector>
#include "constants.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint>& indices, const std::vector<std::shared_ptr<Texture>>& textures) noexcept
	: vertices(vertices),
	indices(indices),
	textures(textures)
{
	InitBuffers();
}

Mesh::Mesh(const Mesh& mesh) noexcept
	: vertices(mesh.vertices),
	indices(mesh.indices),
	textures(mesh.textures)
{
	InitBuffers();
}

Mesh::Mesh(Mesh&& mesh) noexcept
	: vertices(std::move(mesh.vertices)),
	indices(std::move(mesh.indices)),
	textures(std::move(mesh.textures))
{
	VAO = mesh.VAO;
	mesh.VAO = 0;
}

void Mesh::Render(const Shader& shader) const
{
	for (uint i = 0; i < (uint)textures.size(); i++)
	{
		if (textures[i]->type == names::textures::diffuse)
		{
			GLCall(glActiveTexture(GL_TEXTURE0 + i));
			GLCall(glBindTexture(GL_TEXTURE_2D, textures[i]->id));
			shader.SetDiffuseTexture(i);
		}
	}

	GLCall(glBindVertexArray(VAO));
	GLCall(glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0));
	GLCall(glBindVertexArray(0));
}

void Mesh::DepthRender() const
{
	GLCall(glBindVertexArray(VAO));
	GLCall(glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0));
	GLCall(glBindVertexArray(0));
}

void Mesh::InitBuffers()
{
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glBindVertexArray(VAO));

	uint VBO, EBO;
	GLCall(glGenBuffers(1, &VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW));

	GLCall(glGenBuffers(1, &EBO));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)));

	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));

	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords)));

	GLCall(glBindVertexArray(0));
}
