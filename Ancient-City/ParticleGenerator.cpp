#include "ParticleGenerator.h"

#include <random>
#include "Model.h"

ParticleGenerator::ParticleGenerator(std::shared_ptr<Model> particleModel)
	: particleModel(particleModel),
	position(glm::vec3(0.0f)),
	spawnDelay(0.1f),
	speedModifier(2.0f),
	lifeTime(2.0f)
{
	// empty
}

void ParticleGenerator::RenderParticles(Shader& particleShader) const
{
	particleShader.SetVec3("ParticleGeneratorPosition", position);
	for (const auto& particle : particles)
	{
		particleShader.SetVec3("ParticleOffset", particle.offset);
		particleModel->Render(particleShader);
	}
}

void ParticleGenerator::SpawnParticles(float deltaTime)
{
	static std::random_device seed;
	static std::mt19937 generator(seed());
	static std::normal_distribution<float> distribution = std::normal_distribution<float>(0.0f, 0.2f);

	static float totalTime = 0.0f;
	static uint particlesSpawned = 0;

	totalTime += deltaTime;
	uint newParticlesCount = totalTime / spawnDelay - particlesSpawned;
	particlesSpawned += newParticlesCount;

	for (; newParticlesCount > 0; newParticlesCount--)
	{
		particles.emplace_back(Particle(
			speedModifier * glm::vec3(distribution(generator), 1.0f, distribution(generator)),
			lifeTime));
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
