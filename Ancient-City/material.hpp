#pragma once

#include "utils.h"

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	uint textureAmbientID;
	uint textureDiffuseID;
	uint textureSpecularID;
};
