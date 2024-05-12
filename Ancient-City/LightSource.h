#pragma once

#include <functional>

#include "Model.h"

class LightSource
{
public:
	LightSource() = default;
	LightSource(const Model& model);
	LightSource(Model&& model) : model(std::move(model)) {}

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

	inline glm::mat4 GetLightViewMatrix() const { return glm::lookAt(model.GetPosition(), lightDirection, glm::vec3(0.0f, 1.0f, 0.0f)); }
	inline glm::mat4 GetLightProjectionMatrix() const { return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlaneDistance, farPlaneDistance); }
	inline glm::mat4 GetLightSpaceMatrix() const { return GetLightProjectionMatrix() * GetLightViewMatrix(); }

	inline void SetNearPlaneDistance(float nearPlaneDistance) { this->nearPlaneDistance = nearPlaneDistance; }
	inline void SetFarPlaneDistance(float farPlaneDistance) { this->farPlaneDistance = farPlaneDistance; }
	inline void SetLightDirection(const glm::vec3& lightDirection) { this->lightDirection = glm::normalize(-lightDirection); }

	inline glm::vec3 GetPosition() const { return model.GetPosition(); }
	inline void SetPosition(const glm::vec3& position) { model.SetPosition(position); }

	void CreateShadowMap(Shader& shadowShaders, const std::vector<Model*>& models) const;

private:
	void InitShadowMap();

public:
	Model model;

	static inline const uint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

private:
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float ambientStrength = 0.3f;
	float specularStrength = 0.5f;
	float diffuseStrength = 0.5f;
	int specularExponent = 32;

	float nearPlaneDistance = 0.1f;
	float farPlaneDistance = 7.5f;
	glm::vec3 lightDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	uint depthMap = 0;
	uint depthMapFBO = 0;
};