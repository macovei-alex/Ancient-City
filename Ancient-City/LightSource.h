#pragma once

#include "Model.h"

class LightSource
{
public:
	LightSource() = default;
	inline LightSource(const Model& model) : model(model) {}
	inline LightSource(Model&& model) : model(std::move(model)) {}

	inline float GetAmbientStrength() const { return ambientStrength; }
	inline float GetSpecularStrength() const { return specularStrength; }
	inline float GetDiffuseStrength() const { return diffuseStrength; }
	inline int GetSpecularExponent() const { return specularExponent; }
	inline glm::vec3 GetColor() const { return lightColor; }

	inline void SetAmbientStrength(float ambientStrength) { this->ambientStrength = ambientStrength; }
	inline void SetSpecularStrength(float specularStrength) { this->specularStrength = specularStrength; }
	inline void SetDiffuseStrength(float diffuseStrength) { this->diffuseStrength = diffuseStrength; }
	inline void SetSpecularExponent(int specularExponent) { this->specularExponent = specularExponent; }
	inline void SetLightColor(const glm::vec3& lightColor) { this->lightColor = lightColor; }

	void AddAmbientStrength(float difference);
	void AddSpecularStrength(float difference);
	void AddDiffuseStrength(float difference);
	void MultiplySpecularExponent(float difference);

	inline glm::vec3 GetPosition() const { return model.GetPosition(); }
	inline void SetPosition(const glm::vec3& position) { model.SetPosition(position); }

public:
	Model model;

private:
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	float ambientStrength = 0.3f;
	float specularStrength = 0.5f;
	float diffuseStrength = 0.5f;
	int specularExponent = 32;
};