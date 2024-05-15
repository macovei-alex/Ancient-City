#pragma once

#include "Shader.h"
#include "ShadowMap.h"

#include <glm.hpp>
#include <vector>

class DirectionalLightSource
{
public:
	DirectionalLightSource();

	inline float GetAmbientStrength() const { return ambientStrength; }
	inline float GetDiffuseStrength() const { return diffuseStrength; }
	inline float GetSpecularStrength() const { return specularStrength; }
	inline int GetSpecularExponent() const { return specularExponent; }
	inline glm::vec3 GetColor() const { return lightColor; }
	inline glm::vec3 GetDirection() const { return lightDirection; }
	inline const ShadowMap& GetShadowMap() { return shadowMap; }
	virtual glm::vec3 GetPosition() const = 0;

	inline void SetAmbientStrength(float ambientStrength) { this->ambientStrength = ambientStrength; }
	inline void SetDiffuseStrength(float diffuseStrength) { this->diffuseStrength = diffuseStrength; }
	inline void SetSpecularStrength(float specularStrength) { this->specularStrength = specularStrength; }
	inline void SetSpecularExponent(int specularExponent) { this->specularExponent = specularExponent; }
	inline void SetLightColor(const glm::vec3& lightColor) { this->lightColor = lightColor; }
	inline void SetNearPlaneDistance(float nearPlaneDistance) { this->nearPlaneDistance = nearPlaneDistance; }
	inline void SetFarPlaneDistance(float farPlaneDistance) { this->farPlaneDistance = farPlaneDistance; }
	inline void SetLightDirection(const glm::vec3& lightDirection) { this->lightDirection = glm::normalize(lightDirection); }

	void AddAmbientStrength(float difference);
	void AddSpecularStrength(float difference);
	void AddDiffuseStrength(float difference);
	void MultiplySpecularExponent(float difference);
	void RotateDirection(float x, float y, float z);
	void RotateDirection(const glm::vec3& rotation);

	virtual glm::mat4 GetLightViewMatrix() const;
	glm::mat4 GetLightProjectionMatrix() const;
	glm::mat4 GetLightSpaceMatrix() const;

	void WriteToShadowMap(const Shader& shadowShaders, const std::vector<Model*>& models) const;

protected:
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float ambientStrength = 0.3f;
	float diffuseStrength = 0.3f;
	float specularStrength = 0.3f;
	int specularExponent = 32;

	float nearPlaneDistance = 0.0f;
	float farPlaneDistance = 1000.0f;
	glm::vec3 lightDirection = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
	ShadowMap shadowMap;

protected:
	static const uint SHADOW_WIDTH, SHADOW_HEIGHT;
};