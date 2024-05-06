#include "Mesh.h"

#include <vector>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint>& indices, const std::vector<Texture>& textures)
	: vertices(vertices),
	indices(indices),
	textures(textures)
{
	InitBuffers();
}

Mesh::Mesh(const Mesh& mesh)
	: vertices(mesh.vertices),
	indices(mesh.indices),
	textures(mesh.textures),
	VAO(mesh.VAO),
	VBO(mesh.VBO),
	EBO(mesh.EBO)
{
	// empty
}

Mesh::Mesh(Mesh&& mesh)
	: vertices(std::move(mesh.vertices)),
	indices(std::move(mesh.indices)),
	textures(std::move(mesh.textures)),
	VAO(mesh.VAO),
	VBO(mesh.VBO),
	EBO(mesh.EBO)
{
	mesh.VAO = 0;
	mesh.VBO = 0;
	mesh.EBO = 0;
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
		GLCall(glGetUniformLocation(shader.GetID(), textureName.c_str()));

		// now set the sampler to the correct texture unit
		GLuint location = glGetUniformLocation(shader.GetID(), textureName.c_str());
		GLCall(glUniform1i(location, (int)i));

		// and finally bind the texture
		GLCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
	}

	// draw mesh
	GLCall(glBindVertexArray(VAO));
	GLCall(glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0));
	GLCall(glBindVertexArray(0));

	GLCall(glActiveTexture(GL_TEXTURE0));
}

void Mesh::InitBuffers()
{
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &VBO));
	GLCall(glGenBuffers(1, &EBO));

	GLCall(glBindVertexArray(VAO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW));
	
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)));

	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));

	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords)));

	/*
	
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
	
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneIDs));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));
	*/

	GLCall(glBindVertexArray(0));
}
