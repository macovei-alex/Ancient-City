#pragma once

#include <array>
#include <string>

#include "Shader.h"
#include "Camera.h"

class Skybox
{
public:
	Skybox(const std::string& textureDirectory);
	~Skybox() = default;

	void Render(Shader& skyboxShader, const Camera& camera) const;

private:
	unsigned int VAO, VBO, EBO, textureID;

	static const std::array<float, 24> vertices;
	static const std::array<unsigned int, 36> indices;
	static const std::array<std::string, 6> facesCubemap;
};

