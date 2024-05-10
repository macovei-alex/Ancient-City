#pragma once

#include "Particle.h"

class ParticleGenerator
{
public:
	ParticleGenerator(const glm::vec3& position, float spawnDelay = 100.0f);
	void RenderParticles(Shader& particleShader) const;
	void SpawnParticles(float deltaTime);
	void MoveParticles(float deltaTime);

private:
	std::shared_ptr<Model> particleModel;
	std::vector<Particle> particles;
	glm::vec3 position;
	float spawnDelay;
};

