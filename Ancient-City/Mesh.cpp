#include "Mesh.h"

#include <vector>
#include "names.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint>& indices, const std::vector<Texture>& textures) noexcept
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

Mesh::~Mesh()
{
	if(VAO != 0)
		GLCall(glDeleteVertexArrays(1, &VAO));
}

void Mesh::Render(const Shader& shader) const
{
	// bind appropriate textures
	uint diffuseNr = 1;
	uint specularNr = 1;
	uint normalNr = 1;
	uint heightNr = 1;
	for (uint i = 0; i < (uint)textures.size(); i++)
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + i)); // active proper texture unit before binding
		std::string number;
		if (textures[i].name == names::textures::diffuse)
		{
			number = std::to_string(diffuseNr++);
		}
		else if (textures[i].name == names::textures::specular)
		{
			number = std::to_string(specularNr++);
		}
		else if (textures[i].name == names::textures::normal)
		{
			number = std::to_string(normalNr++);
		}
		else if (textures[i].name == names::textures::height)
		{
			number = std::to_string(heightNr++);
		}

		std::string textureName = textures[i].name + number;

		// now set the sampler to the correct texture unit
		shader.SetInt(textureName, i);

		// and finally bind the texture
		GLCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
	}

	// draw mesh
	GLCall(glBindVertexArray(VAO));
	GLCall(glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0));
	GLCall(glBindVertexArray(0));
		
	GLCall(glActiveTexture(GL_TEXTURE0));
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
