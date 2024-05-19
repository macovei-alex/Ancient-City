#pragma once

#include <array>
#include <string>

#include "Shader.h"
#include "BaseCamera.h"

class Skybox
{
public:
	Skybox(const std::string& textureDirectory);
	~Skybox() = default;

	void Bind(uint location) const;
	static void RenderCube();

private:
	uint textureID;

	inline static uint VAO = INT_MAX;
	static const std::array<float, 24> vertices;
	static const std::array<uint, 36> indices;
	static const std::array<std::string, 6> facesCubemap;
};

