#pragma once

#include <glm.hpp>

struct LightAttributes
{
	glm::vec3 direction;
	glm::vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;
	int specularExponent;

	LightAttributes()
		: direction(0.0f, 1.0f, 0.0f),
		color(1.0f, 1.0f, 1.0f),
		ambientIntensity(0.3f),
		diffuseIntensity(0.3f),
		specularIntensity(0.3f),
		specularExponent(32)
	{
		// empty
	}
};
