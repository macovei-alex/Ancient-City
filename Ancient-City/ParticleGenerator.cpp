#include "ParticleGenerator.h"

#include <random>
#include <vector>
#include "Model.h"

const std::vector<Vertex> ParticleGenerator::DEFAULT_MODEL_VERTICES = 
{
	Vertex(-0.25f, -0.25f,  0.25f),
	Vertex( 0.25f, -0.25f,  0.25f),
	Vertex( 0.25f,  0.25f,  0.25f),
	Vertex(-0.25f,  0.25f,  0.25f),
	Vertex(-0.25f, -0.25f, -0.25f),
	Vertex( 0.25f, -0.25f, -0.25f),
	Vertex( 0.25f,  0.25f, -0.25f),
	Vertex(-0.25f,  0.25f, -0.25f)
};

const std::vector<uint> ParticleGenerator::DEFAULT_MODEL_INDICES = 
{
	0, 1, 2,
	2, 3, 0,
	1, 5, 6,
	1, 6, 2,
	5, 4, 7,
	5, 7, 6,
	4, 0, 3,
	4, 3, 7,
	0, 1, 5,
	0, 5, 4,
	3, 2, 6,
	3, 6, 7
};

ParticleGenerator::ParticleGenerator()
	: particleModel()
{
	InitMembersDefault();
	particleModel.meshes->push_back(Mesh(DEFAULT_MODEL_VERTICES, DEFAULT_MODEL_INDICES, std::vector<Texture>()));
}

ParticleGenerator::ParticleGenerator(const Model& particleModel)
	: particleModel(particleModel)
{
	InitMembersDefault();
}

void ParticleGenerator::InitMembersDefault()
{
	position = glm::vec3(0.0f);
	spawnDelay = 0.1f;
	speedModifier = 2.0f;
	lifeTime = 2.0f;
}

void ParticleGenerator::RenderParticles(Shader& particleShader) const
{
	particleShader.SetVec3("ParticleGeneratorPosition", position);
	for (const auto& particle : particles)
	{
		particleShader.SetVec3("ParticleOffset", particle.offset);
		particleModel.Render(particleShader);
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
	uint newParticlesCount = (uint)(totalTime / spawnDelay - particlesSpawned);
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
