#pragma once

#include "Model.h"

class LightSource
{
public:
	LightSource() = default;
	LightSource(const Model& model);
	LightSource(Model&& model);

	float GetAmbientStrength() const;
	float GetSpecularStrength() const;
	float GetDiffuseStrength() const;
	int GetSpecularExponent() const;
	glm::vec3 GetColor() const;

	void SetAmbientStrength(float ambientStrength);
	void SetSpecularStrength(float specularStrength);
	void SetDiffuseStrength(float diffuseStrength);
	void SetSpecularExponent(int specularExponent);
	void SetLightColor(const glm::vec3& lightColor);

	void AddAmbientStrength(float difference);
	void AddSpecularStrength(float difference);
	void AddDiffuseStrength(float difference);
	void MultiplySpecularExponent(float difference);

	glm::vec3 GetPosition() const;

public:
	Model model;

private:
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	float ambientStrength = 0.3f;
	float specularStrength = 0.5f;
	float diffuseStrength = 0.5f;
	int specularExponent = 32;
};