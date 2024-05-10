#include "ParticleGenerator.h"

#include <random>

ParticleGenerator::ParticleGenerator(const glm::vec3& position, float spawnDelay)
	: position(position), spawnDelay(spawnDelay)
{
	Particle::InitStaticVAO();
}

void ParticleGenerator::RenderParticles(Shader& particleShader) const
{
	particleShader.SetVec3("ParticleGeneratorPosition", position);
	for (const auto& particle : particles)
		particle.Render(particleShader);
}

void ParticleGenerator::SpawnParticles(float deltaTime)
{
	static std::random_device seed;
	static std::mt19937 generator(seed());
	static std::normal_distribution<float> distribution = std::normal_distribution<float>(0.0f, 0.1f);

	static float totalTime = 0.0f;
	static uint particlesSpawned = 0;

	totalTime += deltaTime;
	uint newParticlesCount = totalTime / spawnDelay - particlesSpawned;
	particlesSpawned += newParticlesCount;

	for (; newParticlesCount > 0; newParticlesCount--)
	{
		//particles.emplace_back(Particle(glm::vec3(distribution(generator), 1.0f, distribution(generator))));
		particles.emplace_back(Particle(glm::vec3(1.0f, 1.0f, 1.0f)));
	}
}

void ParticleGenerator::MoveParticles(float deltaTime)
{
	/*
	particles.erase(
		std::remove_if(particles.begin(), particles.end(),
		[&deadParticles](const Particle& particle) {
			if (particle.IsDead())
			{
				deadParticles++;
				return true;
			}
			return false;
		}), 
		particles.end());

	for (; deadParticles > 0; deadParticles--)
	{
		particles.emplace_back(Particle(glm::vec3(1, 1, 1)));
	}
	*/

	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].Move(deltaTime);
		if (particles[i].IsDead())
		{
			particles.erase(particles.begin() + i);
			i--;
		}
	}
}
