#include "ParticleGenerator.h"

#include <random>

ParticleGenerator::ParticleGenerator(float spawnDelay)
	: spawnDelay(spawnDelay)
{
	Particle::InitStaticVAO();
}

void ParticleGenerator::RenderParticles(Shader& particleShader) const
{
	for (const auto& particle : particles)
		particle.Render(particleShader);
}

void ParticleGenerator::SpawnParticles(float deltaTime)
{
	static std::random_device seed;
	static std::mt19937 generator(seed());
	static std::normal_distribution<float> distribution(0.0f, 1.0f);

	static float totalTime = 0.0f;
	static uint particlesSpawned = 0;

	totalTime += deltaTime;
	uint newParticlesCount = totalTime / spawnDelay - particlesSpawned;

	particlesSpawned += newParticlesCount;
	for (; newParticlesCount > 0; newParticlesCount--)
	{
		// particles.emplace_back(Particle(glm::vec3(distribution(generator), distribution(generator), distribution(generator))));
		particles.emplace_back(Particle(glm::vec3(1, 1, 1)));
	}
}

void ParticleGenerator::MoveParticles(float deltaTime)
{
	for (auto& particle : particles)
	{
		particle.Move(deltaTime);

		if (particle.IsDead())
			particle.Respawn(particle.velocity);
	}
}
