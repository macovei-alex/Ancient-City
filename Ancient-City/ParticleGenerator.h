#pragma once

#include "Particle.h"
#include "Vertex.h"
#include "Model.h"

class ParticleGenerator
{
public:
	ParticleGenerator();
	ParticleGenerator(const Model& particleModel);
	void InitMembersDefault();

	void RenderParticles(Shader& particleShader) const;
	void SpawnParticles(float deltaTime);
	void MoveParticles(float deltaTime);

	inline ParticleGenerator& WithSpawnDelay(float delay) { spawnDelay = delay; return *this; }
	inline ParticleGenerator& WithPosition(const glm::vec3& pos) { position = pos; return *this; }
	inline ParticleGenerator& WithSpeedModifier(float modifier) { speedModifier = modifier; return *this; }
	inline ParticleGenerator& WithLifeTime(float time) { lifeTime = time; return *this; }

private:
	Model particleModel;
	std::vector<Particle> particles;
	glm::vec3 position;
	float spawnDelay;
	float speedModifier;
	float lifeTime;

	static const std::vector<Vertex> DEFAULT_MODEL_VERTICES;
	static const std::vector<uint> DEFAULT_MODEL_INDICES;
};

