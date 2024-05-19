#include "ParticleGenerator.h"

#include "Model.h"

#include <random>
#include <vector>
#include <regex>

const std::vector<Vertex> ParticleGenerator::DEFAULT_MODEL_VERTICES = 
{
	Vertex(-0.5f, -0.5f,  0.5f),
	Vertex( 0.5f, -0.5f,  0.5f),
	Vertex( 0.5f,  0.5f,  0.5f),
	Vertex(-0.5f,  0.5f,  0.5f),
	Vertex(-0.5f, -0.5f, -0.5f),
	Vertex( 0.5f, -0.5f, -0.5f),
	Vertex( 0.5f,  0.5f, -0.5f),
	Vertex(-0.5f,  0.5f, -0.5f)
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

std::vector<ParticleGenerator*> ParticleGenerator::GetFromFile(const std::string& file)
{
	static const std::regex newGenerator(R"(particle generator \d*)");
	static const std::regex comment(R"(#.*$)");
	static const std::regex position(R"(position: (-?\d+\.\d+) (-?\d+\.\d+) (-?\d+\.\d+))");
	static const std::regex spawnDelay(R"(spawn delay: (\d+\.\d+))");
	static const std::regex speedModifier(R"(speed modifier: (\d+\.\d+))");
	static const std::regex lifeTime(R"(lifetime: (\d+\.\d+))");
	static const std::regex particleColor(R"(particle color: (\d+\.\d+) (\d+\.\d+) (\d+\.\d+))");
	static const std::regex particleStartColor(R"(particle start color: (\d+\.\d+) (\d+\.\d+) (\d+\.\d+))");
	static const std::regex particleEndColor(R"(particle end color: (\d+\.\d+) (\d+\.\d+) (\d+\.\d+))");
	static const std::regex scale(R"(scale: (\d+\.\d+))");
	static const std::regex doParticleAlphaFade(R"(do particle alpha fade: (true|false))");

	std::vector<ParticleGenerator*> generators;
	std::ifstream fin(file);
	std::string line;
	size_t lineCounter = 0;

	while (fin.good() && !fin.eof())
	{
		std::getline(fin, line);
		lineCounter++;

		if(line.empty())
			continue;

		try
		{
			if (std::regex_match(line, newGenerator))
			{
				generators.push_back(new ParticleGenerator());
			}
			else if (std::regex_match(line, comment))
			{
				continue;
			}
			else if (std::regex_match(line, position))
			{
				std::smatch match;
				std::regex_search(line, match, position);
				glm::vec3 position = glm::vec3(std::stof(match[1].str()), std::stof(match[2].str()), std::stof(match[3].str()));
				generators.back()->WithPosition(position);
			}
			else if (std::regex_match(line, spawnDelay))
			{
				std::smatch match;
				std::regex_search(line, match, spawnDelay);
				generators.back()->WithSpawnDelay(std::stof(match[1].str()));
			}
			else if (std::regex_match(line, speedModifier))
			{
				std::smatch match;
				std::regex_search(line, match, speedModifier);
				generators.back()->WithSpeedModifier(std::stof(match[1].str()));
			}
			else if (std::regex_match(line, lifeTime))
			{
				std::smatch match;
				std::regex_search(line, match, lifeTime);
				generators.back()->WithLifeTime(std::stof(match[1].str()));
			}
			else if (std::regex_match(line, particleColor))
			{
				std::smatch match;
				std::regex_search(line, match, particleColor);
				glm::vec3 color = glm::vec3(std::stof(match[1].str()), std::stof(match[2].str()), std::stof(match[3].str()));
				generators.back()->WithParticleColor(color.r, color.g, color.b);
			}
			else if (std::regex_match(line, particleStartColor))
			{
				std::smatch match;
				std::regex_search(line, match, particleStartColor);
				glm::vec3 color = glm::vec3(std::stof(match[1].str()), std::stof(match[2].str()), std::stof(match[3].str()));
				generators.back()->WithStartingParticleColor(color.r, color.g, color.b);
			}
			else if (std::regex_match(line, particleEndColor))
			{
				std::smatch match;
				std::regex_search(line, match, particleEndColor);
				glm::vec3 color = glm::vec3(std::stof(match[1].str()), std::stof(match[2].str()), std::stof(match[3].str()));
				generators.back()->WithEndingParticleColor(color.r, color.g, color.b);
			}
			else if (std::regex_match(line, scale))
			{
				std::smatch match;
				std::regex_search(line, match, scale);
				generators.back()->WithScale(std::stof(match[1].str()));
			}
			else if (std::regex_match(line, doParticleAlphaFade))
			{
				std::smatch match;
				std::regex_search(line, match, doParticleAlphaFade);
				generators.back()->WithParticleAlphaFade(match[1].str() == "true");
			}
			else
			{
				LOG(std::format("Invalid line in file ( {} ) at line ( {} ): {}", file, lineCounter, line), Logger::Level::Error);
			}
		}
		catch (const std::exception& e)
		{
			LOG(std::format("Error parsing file ( {} ) at line ( {} ) with contents ( {} ): {}", file, lineCounter, line, e.what()), Logger::Level::Error);
		}
	}

	return generators;
}

ParticleGenerator::ParticleGenerator()
	: particleModel()
{
	InitMembersDefault();
	particleModel.meshes.push_back(std::make_shared<Mesh>(DEFAULT_MODEL_VERTICES, DEFAULT_MODEL_INDICES, std::make_shared<Material>()));
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
	particleStartColor = glm::vec3(1.0f);
	particleEndColor = glm::vec3(1.0f);
	scale = 1.0f;
	doParticleAlphaFade = false;
}

void ParticleGenerator::RenderParticles(const Shader& particleShader) const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	particleShader.SetParticleStartColor(particleStartColor);
	particleShader.SetParticleEndColor(particleEndColor);
	particleShader.SetParticleScale(scale);

	for (const auto& particle : particles)
	{
		float lifePercent = particle.lifeTime / lifeTime;

		particleShader.SetParticlePosition(particle.position);
		particleShader.SetParticleBlendPercent(1 - lifePercent);

		if (doParticleAlphaFade)
		{
			particleShader.SetParticleAlpha(lifePercent);
		}

		particleModel.DepthRender();
	}

	glDisable(GL_BLEND);
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
			position, 
			speedModifier * glm::vec3(distribution(generator), 1.0f, distribution(generator)),
			lifeTime));
	}
}

void ParticleGenerator::MoveParticles(float deltaTime)
{
	std::for_each(particles.begin(),
		particles.end(), 
		[&deltaTime](Particle& particle) { particle.Move(deltaTime); });

	particles.erase(
		std::remove_if(
			particles.begin(),
			particles.end(),
			[](const Particle& particle) { 
				return particle.IsDead();
			}), 
		particles.end());
}

float ParticleGenerator::CalculateAmbientIntensity(float ambient)
{
	return std::min(std::max(0.2f, 5 * ambient), 1.0f);
}

float ParticleGenerator::CalculateDiffuseStrength(float baseDiffuseStrength, const glm::vec3& lightPosition) const
{
	float distance = glm::distance(lightPosition, position);
	return std::min(std::max(0.3f, 5 * baseDiffuseStrength / std::sqrt(distance)), 1.0f);
}
