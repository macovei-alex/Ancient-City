#pragma once

#include "Mesh.h"

#include <vector>

class Model
{
public:
	Model();
	Model(const Model& model);
	Model(Model&& model);

	void Render(const ShaderProgram& shader) const;

	glm::mat4 GetModelMatrix() const;
	glm::vec3 GetPosition() const;

	void SetPosition(const glm::vec3& position);
	void SetScale(const glm::vec3& scale);
	void SetRotation(const glm::vec3& rotation);

	void Translate(float x, float y = 0, float z = 0);
	void Translate(const glm::vec3& translation);

	void Scale(float scale);
	void Scale(const glm::vec3& scale);

	void Rotate(float x, float y = 0, float z = 0);
	void Rotate(const glm::vec3& rotation);

public:
	std::vector<Mesh> meshes;
	std::vector<Texture> textures;

private:
	glm::mat4 modelMatrix;
};