#pragma once

#include "Utils.h"

#include <string>
#include <vector>
#include <glm.hpp>
#include <glew.h>

class Model
{
public:
	Model();
	Model(const std::vector<glm::vec3>& vertices,
		const std::vector<glm::vec3>& colors,
		const std::vector<glm::vec3u>& indices,
		const std::vector<glm::vec3>& normals);
	Model(const std::string& filePath, bool makeCentered = false);
	Model(Model&& model) noexcept;
	Model(const Model&);
	~Model();

	void Render() const;
	void InitBuffers();
	void DestroyBuffers();

	glm::mat4 GetModelMatrix() const;
	glm::vec3 GetPosition() const;

	void SetPosition(const glm::vec3& position);
	void SetScale(const glm::vec3& scale);
	void SetRotation(const glm::vec3& rotation);

	void Translate(const glm::vec3& translation);
	void Scale(const glm::vec3& scale);
	void Rotate(const glm::vec3& rotation);

	void CenterModel();

private:
	void ReadVertices(std::ifstream& fin);
	void ReadColors(std::ifstream& fin);
	void ReadIndices(std::ifstream& fin);

	void CalculateNormals();

private:
public:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec<3, unsigned int>> indices;
	std::vector<glm::vec3> normals;

	GLuint vertexArrayID = 0;
	GLuint vertexBufferID = 0;
	GLuint colorBufferID = 0;
	GLuint indexBufferID = 0;
	GLuint normalBufferID = 0;

	glm::mat4 modelMatrix;
	bool isCentered;
};