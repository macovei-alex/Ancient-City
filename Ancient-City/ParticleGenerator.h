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
	inline ParticleGenerator& WithParticleColor(const glm::vec3& color) { particleColor = color; return *this; }
	inline ParticleGenerator& WithParticleColor(float r, float g, float b) { particleColor = glm::vec3(r, g, b); return *this; }

private:
	Model particleModel;
	std::vector<Particle> particles;
	glm::vec3 position;
	float spawnDelay;
	float speedModifier;
	float lifeTime;
	glm::vec3 particleColor;

	static const std::vector<Vertex> DEFAULT_MODEL_VERTICES;
	static const std::vector<uint> DEFAULT_MODEL_INDICES;
};

