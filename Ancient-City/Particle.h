#pragma once

#include "Shader.h"

class Particle
{
public:
	glm::vec3 position;
	glm::vec3 velocity;
	float lifeTime;

	Particle(const glm::vec3& velocity);
	void Respawn(const glm::vec3& newVelocity);

	inline float GetLifeTime() const { return lifeTime; }
	inline bool IsDead() const { return lifeTime <= 0; }
	inline void Move(float timePassed) { position += velocity * timePassed; lifeTime -= timePassed; }
	void Render(Shader& particleShader) const;

	static void InitStaticVAO();

private:
	inline static uint VAO = 0;
	inline static const float vertices[] =
	{
		// positions          // tex coords
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,   0.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,   0.0f, 1.0f,

		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,   0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,   1.0f, 1.0f
	};
};
