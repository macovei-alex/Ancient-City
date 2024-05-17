#pragma once

#include "utils.h"
#include "constants.h"
#include "texture.hpp"
#include "Shader.h"

struct Material
{
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	int specularExponent;

	std::shared_ptr<Texture> diffuseTexture;
	std::shared_ptr<Texture> specularTexture;

	inline Material() : id(nextID++),
		ambientColor(0.0f), diffuseColor(0.0f), specularColor(0.0f), specularExponent(32),
		diffuseTexture(nullptr), specularTexture(nullptr) {}

	inline bool operator==(const Material& other) const
	{
		return ambientColor == other.ambientColor &&
			diffuseColor == other.diffuseColor &&
			specularColor == other.specularColor &&
			specularExponent == other.specularExponent &&
			diffuseTexture == other.diffuseTexture &&
			specularTexture == other.specularTexture;
	}

	inline bool operator<(const Material& other) const
	{
		return ambientColor.x < other.ambientColor.x;
	}

	inline void Bind(const Shader& shader) const
	{
		shader.SetAmbientColor(ambientColor);
		shader.SetDiffuseColor(diffuseColor);
		shader.SetSpecularColor(specularColor);
		shader.SetSpecularExponent(specularExponent);

		diffuseTexture->Bind(values::textureUnits::diffuse);
		shader.SetDiffuseTexture(values::textureUnits::diffuse);

		specularTexture->Bind(values::textureUnits::specular);
		shader.SetSpecularTexture(values::textureUnits::specular);
	}

	inline uint GetID() const { return id; }

private:
	inline static uint nextID = 1;

private:
	uint id;
};
