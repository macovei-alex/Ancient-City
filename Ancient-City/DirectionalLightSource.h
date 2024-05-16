#pragma once

#include "Shader.h"
#include "ShadowMap.h"
#include "lightAttributes.hpp"

#include <glm.hpp>
#include <vector>

class DirectionalLightSource
{
public:
	DirectionalLightSource();

	inline const ShadowMap& GetShadowMap() { return shadowMap; }
	virtual glm::vec3 GetPosition() const = 0;

	inline void SetNearPlaneDistance(float nearPlaneDistance) { this->nearPlaneDistance = nearPlaneDistance; }
	inline void SetFarPlaneDistance(float farPlaneDistance) { this->farPlaneDistance = farPlaneDistance; }

	void AddAmbientIntensity(float difference);
	void AddSpecularIntensity(float difference);
	void AddDiffuseIntensity(float difference);
	void MultiplySpecularExponent(float difference);
	void RotateDirection(float x, float y, float z);
	void RotateDirection(const glm::vec3& rotation);

	virtual glm::mat4 GetLightViewMatrix() const;
	glm::mat4 GetLightProjectionMatrix() const;
	glm::mat4 GetLightSpaceMatrix() const;

	void WriteToShadowMap(const Shader& shadowShaders, const std::vector<Model*>& models) const;

public:
	LightAttributes light;

protected:
	float nearPlaneDistance = 0.0f;
	float farPlaneDistance = 1000.0f;
	ShadowMap shadowMap;

protected:
	static const uint SHADOW_WIDTH, SHADOW_HEIGHT;
};