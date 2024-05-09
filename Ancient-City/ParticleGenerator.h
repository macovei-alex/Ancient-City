#pragma once

#include "utils.h"
#include "particle.hpp"

class ParticleGenerator
{
public:
	ParticleGenerator(float spawnParticleTimeOffset = 0.1f);
	void TrySpawnParticles(float timePassed);
	void MoveParticles(float timePassed);

private:
	std::vector<Particle> particles;
	float particleSpawnTimeOffset;
};

