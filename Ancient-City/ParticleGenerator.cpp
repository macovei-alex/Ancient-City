#include "ParticleGenerator.h"

#include <random>

ParticleGenerator::ParticleGenerator(float spawnDelay)
	: spawnDelay(spawnDelay)
{
	// empty
}

void ParticleGenerator::TrySpawnParticles(float timePassed)
{
	static std::random_device seed;
	static std::mt19937 generator(seed());
	static std::normal_distribution<float> distribution(0.0f, 1.0f);

	static float totalTime = 0.0f;
	static uint particlesSpawned = 0;

	totalTime += timePassed;
	uint newParticlesCount = totalTime / spawnDelay - particlesSpawned;

	particlesSpawned += newParticlesCount;
	for (; newParticlesCount > 0; newParticlesCount--)
	{
		// particles.emplace_back(Particle(glm::vec3(distribution(generator), distribution(generator), distribution(generator))));
		particles.emplace_back(Particle(glm::vec3(1, 1, 1)));
	}
}

void ParticleGenerator::MoveParticles(float timePassed)
{
	for (auto& particle : particles)
	{
		particle.Move(timePassed);

		if (particle.IsDead())
			particle.Respawn(particle.velocity);
	}
}
