#pragma once

#include "Shader.h"

struct Particle
{
	glm::vec3 position;
	glm::vec3 velocity;
	float lifeTime;

	inline Particle(const glm::vec3& position, const glm::vec3& velocity, float lifeTime)
		: position(position), velocity(velocity), lifeTime(lifeTime)
	{}

	inline bool IsDead() const
	{
		return lifeTime <= 0;
	}

	inline void Move(float deltaTime)
	{
		position += velocity * deltaTime;
		lifeTime -= deltaTime;
	}
};
