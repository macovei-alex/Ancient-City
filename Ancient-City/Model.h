#pragma once

#include "Utils.h"
#include "Mesh.h"
#include "vertex.h"
#include "texture.h"

class Model
{
public:
	Model();

	void Render(ShaderProgram& shader) const;

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
	std::vector<Mesh> meshes;
	std::vector<Texture> textures;

	GLuint vertexArrayID = 0;
	GLuint vertexBufferID = 0;
	GLuint colorBufferID = 0;
	GLuint indexBufferID = 0;
	GLuint normalBufferID = 0;

	glm::mat4 modelMatrix;
};