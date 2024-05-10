#pragma once

#include "Shader.h"

struct Particle
{
	glm::vec3 offset;
	glm::vec3 velocity;
	float lifeTime;

	inline Particle(const glm::vec3& velocity, float lifeTime)
	{
		Reset(velocity, lifeTime);
	}

	inline void Reset(const glm::vec3& newVelocity, float newLifeTime)
	{
		offset = glm::vec3(0.0f);
		velocity = newVelocity;
		lifeTime = newLifeTime;
	}

	inline bool IsDead() const
	{
		return lifeTime <= 0;
	}

	inline void Move(float deltaTime)
	{
		offset += velocity * deltaTime;
		lifeTime -= deltaTime;
	}
};
