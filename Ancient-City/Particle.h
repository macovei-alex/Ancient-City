#pragma once

#include "Shader.h"

class Particle
{
public:
	glm::vec3 offset;
	glm::vec3 velocity;
	float lifeTime;

	Particle(const glm::vec3& velocity);
	void Respawn(const glm::vec3& newVelocity);

	inline float GetLifeTime() const { return lifeTime; }
	inline bool IsDead() const { return lifeTime <= 0; }
	inline void Move(float deltaTime) { offset += velocity * deltaTime; lifeTime -= deltaTime; }
	void Render(Shader& particleShader) const;

	static void InitStaticVAO();

private:
	static const float vertices[];
	static uint VAO;
};
