#include "Model.h"

#include "utils.h"

#include <gtc/matrix_transform.hpp>

Model::Model()
	: modelMatrix(glm::mat4(1.0f))
{
	// empty
}

Model::Model(const Model& model)
	: modelMatrix(model.modelMatrix),
	meshes(model.meshes),
	textures(model.textures)
{
	// empty
}

Model::Model(Model&& model)
	: modelMatrix(model.modelMatrix),
	meshes(std::move(model.meshes)),
	textures(std::move(model.textures))
{
	// empty
}

glm::mat4 Model::GetModelMatrix() const
{
	return modelMatrix;
}

glm::vec3 Model::GetPosition() const
{
	return glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
}

void Model::SetPosition(const glm::vec3& position)
{
	modelMatrix[3][0] = position.x;
	modelMatrix[3][1] = position.y;
	modelMatrix[3][2] = position.z;
}

void Model::SetScale(const glm::vec3& scale)
{
	modelMatrix[0][0] = scale.x;
	modelMatrix[1][1] = scale.y;
	modelMatrix[2][2] = scale.z;
}

void Model::SetRotation(const glm::vec3& rotation)
{
	modelMatrix = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Model::Translate(const glm::vec3& translation)
{
	modelMatrix = glm::translate(modelMatrix, translation);
}

void Model::Translate(float x, float y, float z)
{
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
}

void Model::Scale(float scale)
{
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, scale, scale));
}

void Model::Scale(const glm::vec3& scale)
{
	modelMatrix = glm::scale(modelMatrix, scale);
}

void Model::Rotate(float x, float y, float z)
{
	modelMatrix = glm::rotate(modelMatrix, x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, z, glm::vec3(0.0f, 0.0f, 1.0f));

}

void Model::Rotate(const glm::vec3& rotation)
{
	modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Model::Render(const ShaderProgram& shaders) const
{
	for (uint i = 0; i < meshes.size(); i++)
		meshes[i].Render(shaders);
}