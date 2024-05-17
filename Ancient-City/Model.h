#pragma once

#include "Mesh.h"
#include <gtc/matrix_transform.hpp>
#include <vector>

class Model
{
public:
	inline Model() : modelMatrix(1.0f), meshes(std::vector<std::shared_ptr<Mesh>>()), textures(std::vector<std::shared_ptr<Texture>>()) {}
	Model(const Model& model) = default;
	Model(Model&& model) = default;

	void Render(const Shader& shader) const;
	void DepthRender() const;

	inline glm::vec3 GetPosition() const { return glm::vec3(modelMatrix[3]); }

	void SetPosition(const glm::vec3& position);
	void SetScale(const glm::vec3& scale);
	void SetRotation(const glm::vec3& rotation);

	inline void Translate(float x, float y, float z) { Translate(glm::vec3(x, y, z)); }
	inline void Translate(const glm::vec3& translation) { modelMatrix = glm::translate(modelMatrix, translation); }
	
	inline void Scale(float scale) { Scale(glm::vec3(scale, scale, scale)); }
	inline void Scale(const glm::vec3& scale) { modelMatrix = glm::scale(modelMatrix, scale); }

	void Rotate(float x, float y = 0, float z = 0);
	void Rotate(const glm::vec3& rotation);

public:
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<std::shared_ptr<Texture>> textures;
	glm::mat4 modelMatrix;
};