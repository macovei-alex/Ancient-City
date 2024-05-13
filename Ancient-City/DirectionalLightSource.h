#pragma once

#include "Shader.h"

#include <glm.hpp>
#include <vector>

class DirectionalLightSource
{
public:
	DirectionalLightSource() = default;

	inline float GetAmbientStrength() const { return ambientStrength; }
	inline float GetSpecularStrength() const { return specularStrength; }
	inline float GetDiffuseStrength() const { return diffuseStrength; }
	inline int GetSpecularExponent() const { return specularExponent; }
	inline glm::vec3 GetColor() const { return lightColor; }
	inline glm::vec3 GetDirection() const { return lightDirection; }

	inline void SetAmbientStrength(float ambientStrength) { this->ambientStrength = ambientStrength; }
	inline void SetSpecularStrength(float specularStrength) { this->specularStrength = specularStrength; }
	inline void SetDiffuseStrength(float diffuseStrength) { this->diffuseStrength = diffuseStrength; }
	inline void SetSpecularExponent(int specularExponent) { this->specularExponent = specularExponent; }
	inline void SetLightColor(const glm::vec3& lightColor) { this->lightColor = lightColor; }
	void SetNearPlaneDistance(float nearPlaneDistance) { this->nearPlaneDistance = nearPlaneDistance; }
	void SetFarPlaneDistance(float farPlaneDistance) { this->farPlaneDistance = farPlaneDistance; }
	void SetLightDirection(const glm::vec3& lightDirection) { this->lightDirection = glm::normalize(-lightDirection); }

	void AddAmbientStrength(float difference);
	void AddSpecularStrength(float difference);
	void AddDiffuseStrength(float difference);
	void MultiplySpecularExponent(float difference);
	void RotateDirection(float x, float y, float z);
	void RotateDirection(const glm::vec3& rotation);

	glm::mat4 GetLightViewMatrix() const;
	glm::mat4 GetLightProjectionMatrix() const;
	glm::mat4 GetLightSpaceMatrix() const;

	void CreateShadowMap(Shader& shadowShaders, const std::vector<Model*>& models) const;

private:
	void InitShadowMap();

public:
	static inline const uint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

private:
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float ambientStrength = 0.3f;
	float specularStrength = 0.5f;
	float diffuseStrength = 0.5f;
	int specularExponent = 32;

	float nearPlaneDistance = 0.1f;
	float farPlaneDistance = 10.0f;
	glm::vec3 lightDirection = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
	uint depthMap = 0;
	uint depthMapFBO = 0;
};