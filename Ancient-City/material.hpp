#pragma once

#include "utils.h"
#include "texture.hpp"

struct Material
{
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float specularExponent;

	std::shared_ptr<Texture> diffuseTexture;
	std::shared_ptr<Texture> specularTexture;
};
