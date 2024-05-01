#pragma once

#include "Utils.h"
#include "Mesh.h"
#include "vertex.h"
#include "texture.h"

class Model
{
public:
	Model();

	void Render(const ShaderProgram& shader) const;

	glm::mat4 GetModelMatrix() const;
	glm::vec3 GetPosition() const;

	void SetPosition(const glm::vec3& position);
	void SetScale(const glm::vec3& scale);
	void SetRotation(const glm::vec3& rotation);

	void Translate(const glm::vec3& translation);
	void Scale(const glm::vec3& scale);
	void Rotate(const glm::vec3& rotation);

private:
public:
	glm::mat4 modelMatrix;
	std::vector<Mesh> meshes;
	std::vector<Texture> textures;
};