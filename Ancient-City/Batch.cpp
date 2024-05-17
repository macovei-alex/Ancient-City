#include "Batch.h"

#include "Model.h"
#include "constants.h"
#include <map>

std::vector<Batch*> Batch::SplitToBatches(const std::vector<Model*>& models)
{
	std::vector<Batch*> batches;
	std::map<int, std::pair<std::vector<Mesh*>, std::vector<glm::mat4>>> splitMeshes;
	splitMeshes.insert({ -1, std::pair<std::vector<Mesh*>, std::vector<glm::mat4>>() });

	for(Model* model : models)
	{
		for (size_t i = 0; i < model->meshes->size(); i++)
		{
			Mesh* mesh = &model->meshes->at(i);

			int textureId = mesh->textures.size() == 0 ? -1 : mesh->textures[0].id;

			if(splitMeshes.find(textureId) == splitMeshes.end())
				splitMeshes.insert({ textureId, std::pair<std::vector<Mesh*>, std::vector<glm::mat4>>() });

			auto& [meshes, matrices] = splitMeshes[textureId];
			meshes.push_back(mesh);
			matrices.push_back(model->modelMatrix);
		}
	}

	batches.reserve(splitMeshes.size());

	for(auto& [textureId, pair] : splitMeshes)
		if(textureId != -1)
			batches.push_back(new Batch(pair.first, pair.second));

	return batches;
}

Batch::Batch(const std::vector<Mesh*>& meshes, const std::vector<glm::mat4>& matrices)
{
	std::vector<Vertex> vertices;
	std::vector<uint> indices;

	size_t vertexCounter = 0;
	for(Mesh* mesh : meshes)
	{
		vertexCounter += mesh->vertices.size();
		indexCount += mesh->indices.size();
	}

	vertices.reserve(vertexCounter);
	indices.reserve(indexCount);
	uint indexOffset = 0;

	for (size_t i = 0; i < meshes.size(); i++)
	{
		for (size_t j = 0; j < meshes[i]->vertices.size(); j++)
		{
			Vertex newVertex = meshes[i]->vertices[j];
			newVertex.position = matrices[i] * meshes[i]->vertices[j].position;

			vertices.push_back(newVertex);
		}

		for (uint index : meshes[i]->indices)
		{
			uint newIndex = index + indexOffset;
			indices.push_back(newIndex);

			// remove the weird artifact spanning across half the town
			if (indexCount == 56286 && (newIndex == 37027 || newIndex == 37028 || newIndex == 37029))
				indices.pop_back();
		}
		
		indexOffset += (uint)meshes[i]->vertices.size();
	}

	textures = meshes[0]->textures;
	InitBuffers(vertices, indices, meshes[0]->textures);
}

Batch::Batch(Batch&& other) noexcept
	: VAO(other.VAO),
	indexCount(other.indexCount),
	textures(std::move(other.textures))
{
	other.VAO = -1;
}

Batch& Batch::operator=(Batch&& other) noexcept
{
	if (this != &other)
	{
		VAO = other.VAO;
		indexCount = other.indexCount;
		textures = std::move(other.textures);

		other.VAO = -1;
	}

	return *this;
}

Batch::~Batch()
{
	if(VAO != -1)
		GLCall(glDeleteVertexArrays(1, &VAO));
}

void Batch::Render(const Shader& shader) const
{
	for (uint i = 0; i < (uint)textures.size(); i++)
	{
		if (textures[i].type == names::textures::diffuse)
		{
			GLCall(glActiveTexture(GL_TEXTURE0 + i));
			GLCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
			shader.SetDiffuseTexture(i);
		}
	}

	GLCall(glBindVertexArray(VAO));
	GLCall(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0));
	GLCall(glBindVertexArray(0));
	
	for(uint i = 0; i < (uint)textures.size(); i++)
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + i));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
}

void Batch::DepthRender() const
{
	GLCall(glBindVertexArray(VAO));
	GLCall(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0));
	GLCall(glBindVertexArray(0));
}

void Batch::InitBuffers(const std::vector<Vertex>& vertices, const std::vector<uint>& indices, const std::vector<Texture>& texture)
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
