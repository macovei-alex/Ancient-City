#pragma once

#include "Particle.h"

class ParticleGenerator
{
public:
	ParticleGenerator(float spawnDelay = 0.1f);
	void RenderParticles(Shader& particleShader) const;
	void SpawnParticles(float timePassed);
	void MoveParticles(float timePassed);

private:
	std::vector<Particle> particles;
	float spawnDelay;
};

