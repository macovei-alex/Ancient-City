#pragma once

#include "particle.hpp"
#include "Vertex.hpp"
#include "Model.h"

class ParticleGenerator
{
public:
	static std::vector<ParticleGenerator*> GetFromFile(const std::string& file);

public:
	ParticleGenerator();
	ParticleGenerator(const Model& particleModel);
	ParticleGenerator(const ParticleGenerator& other) = default;
	ParticleGenerator& operator=(const ParticleGenerator& other) = default;
	void InitMembersDefault();

	void RenderParticles(const Shader& particleShader) const;
	void SpawnParticles(float deltaTime);
	void MoveParticles(float deltaTime);

	inline ParticleGenerator& WithSpawnDelay(float delay) { spawnDelay = delay; return *this; }
	inline ParticleGenerator& WithPosition(const glm::vec3& pos) { position = pos; return *this; }
	inline ParticleGenerator& WithPosition(float x, float y, float z) { position = glm::vec3(x, y, z); return *this; }
	inline ParticleGenerator& WithSpeedModifier(float modifier) { speedModifier = modifier; return *this; }
	inline ParticleGenerator& WithLifeTime(float time) { lifeTime = time; return *this; }
	inline ParticleGenerator& WithParticleColor(float r, float g, float b) { particleStartColor = particleEndColor = glm::vec3(r, g, b); return *this; }
	inline ParticleGenerator& WithStartingParticleColor(float r, float g, float b) { particleStartColor = glm::vec3(r, g, b); return *this; }
	inline ParticleGenerator& WithEndingParticleColor(float r, float g, float b) { particleEndColor = glm::vec3(r, g, b); return *this; }
	inline ParticleGenerator& WithScale(float scale) { this->scale = scale; return *this; }
	inline ParticleGenerator& WithParticleAlphaFade(bool doAlphaFade) { doParticleAlphaFade = doAlphaFade; return *this; }

	static float CalculateAmbientIntensity(float ambientStrength);
	float CalculateDiffuseStrength(float baseDiffuseStrength, const glm::vec3& lightPosition) const;

private:
	Model particleModel;
	std::vector<Particle> particles;

	glm::vec3 position;
	float spawnDelay;
	float speedModifier;
	float lifeTime;
	glm::vec3 particleStartColor;
	glm::vec3 particleEndColor;
	float scale;
	bool doParticleAlphaFade;

	static const std::vector<Vertex> DEFAULT_MODEL_VERTICES;
	static const std::vector<uint> DEFAULT_MODEL_INDICES;
};

