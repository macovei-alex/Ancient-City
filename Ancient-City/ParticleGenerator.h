#pragma once

#include "Particle.h"

class ParticleGenerator
{
public:
	ParticleGenerator(float spawnDelay = 0.1f);
	void TrySpawnParticles(float timePassed);
	void MoveParticles(float timePassed);

private:
	std::vector<Particle> particles;
	float spawnDelay;
};

