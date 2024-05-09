#pragma once

#include "Particle.h"

class ParticleGenerator
{
public:
	ParticleGenerator(const glm::vec3& position, float spawnDelay = 0.1f);
	void RenderParticles(Shader& particleShader) const;
	void SpawnParticles(float deltaTime);
	void MoveParticles(float deltaTime);

private:
	std::vector<Particle> particles;
	glm::vec3 position;
	float spawnDelaySeconds;
};

