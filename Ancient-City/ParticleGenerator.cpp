#include "ParticleGenerator.h"

#include <random>

ParticleGenerator::ParticleGenerator(float particleSpawnTimeOffset)
	: particleSpawnTimeOffset(particleSpawnTimeOffset)
{
	// empty
}

void ParticleGenerator::TrySpawnParticles(float timePassed)
{
	static std::random_device seed;
	static std::mt19937 generator(seed());
	static std::normal_distribution<float> distribution(0.0f, 1.0f);

	static float timeSinceLastSpawn = 0.0f;

	timeSinceLastSpawn += timePassed;
	if (timeSinceLastSpawn >= particleSpawnTimeOffset)
	{
		timeSinceLastSpawn = 0.0f;
		particles.emplace_back(Particle(glm::vec3(distribution(generator), distribution(generator), distribution(generator))));
	}
}

void ParticleGenerator::MoveParticles(float timePassed)
{
	for (auto& particle : particles)
	{
		particle.Move(timePassed);
	}
}
