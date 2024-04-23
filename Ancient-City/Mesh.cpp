#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint>& indices, const std::vector<Texture>& textures)
	: vertexCount((uint)vertices.size()),
	indexCount((uint)indices.size()),
	vertices(new Vertex[vertexCount]),
	indices(new uint[indexCount]),
	textures(textures)
{
	InitBuffers();
}

Mesh::Mesh(uint vertexCount, std::shared_ptr<Vertex> vertices, uint indexCount, std::shared_ptr<uint> indices, const std::vector<Texture>& textures)
	: vertexCount(vertexCount),
	indexCount(indexCount),
	vertices(vertices),
	indices(indices),
	textures(textures)
{
	InitBuffers();
}

void Mesh::Render(ShaderProgram& shader) const
{
	// bind appropriate textures
	uint diffuseNr = 1;
	uint specularNr = 1;
	uint normalNr = 1;
	uint heightNr = 1;
	for (size_t i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + (GLenum)i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to string
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to string
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to string

		// now set the sampler to the correct texture unit
		GLCall(glUniform1i(glGetUniformLocation(shader.GetID(), (name + number).c_str()), (int)i));
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	// draw mesh
	GLCall(glBindVertexArray(VAO));
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
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
	GLCall(glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), &vertices.get()[0], GL_STATIC_DRAW));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint), &indices.get()[0], GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));

	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));

	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords)));

	// glEnableVertexAttribArray(3);
	// glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	// glEnableVertexAttribArray(4);
	// glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	// glEnableVertexAttribArray(5);
	// glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneIDs));

	// glEnableVertexAttribArray(6);
	// glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));

	GLCall(glBindVertexArray(0));
}
