#include "Mesh.h"

#include <vector>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint>& indices, const std::vector<Texture>& textures) noexcept
	: vertices(vertices),
	indices(indices),
	textures(textures),
	vao(vertices, indices)
{
	// empty
}

Mesh::Mesh(const Mesh& mesh) noexcept
	: vertices(mesh.vertices),
	indices(mesh.indices),
	textures(mesh.textures),
	vao(mesh.vao)
{
	// empty
}

Mesh::Mesh(Mesh&& mesh) noexcept
	: vertices(std::move(mesh.vertices)),
	indices(std::move(mesh.indices)),
	textures(std::move(mesh.textures)),
	vao(std::move(mesh.vao))
{
}

void Mesh::Render(const Shader& shader) const
{
	// bind appropriate textures
	uint diffuseNr = 1;
	uint specularNr = 1;
	uint normalNr = 1;
	uint heightNr = 1;
	for (size_t i = 0; i < textures.size(); i++)
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + (GLenum)i)); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		if (textures[i].name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
		}
		else if (textures[i].name == "texture_specular")
		{
			number = std::to_string(specularNr++);
		}
		else if (textures[i].name == "texture_normal")
		{
			number = std::to_string(normalNr++);
		}
		else if (textures[i].name == "texture_height")
		{
			number = std::to_string(heightNr++);
		}

		std::string textureName = textures[i].name + number;

		// now set the sampler to the correct texture unit
		shader.SetInt(textureName, static_cast<int>(i));

		// and finally bind the texture
		GLCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
	}

	// draw mesh
	vao.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0));
	vao.Unbind();

	GLCall(glActiveTexture(GL_TEXTURE0));
}

/*
void Mesh::InitBuffers()
{
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
	
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneIDs));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));

	GLCall(vao.Unbind());
}
*/
