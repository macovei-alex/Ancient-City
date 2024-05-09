#pragma once

#include "utils.h"
#include "Shader.h"

struct Particle
{
	glm::vec3 offset;
	glm::vec3 velocity;
	float maxDistance;
	float scale;
	uint textureId;

	inline Particle(const glm::vec3& velocity, float maxDistance = 1, float scale = 1)
	{
		this->offset = glm::vec3(0);
		this->velocity = velocity;
		this->maxDistance = maxDistance;
		this->scale = scale;
		this->textureId = 0;
	}

	inline float CalculateLife()
	{
		return maxDistance - glm::length(offset);
	}

	inline bool IsDead()
	{
		return CalculateLife() <= 0.0f;
	}

	inline void Move(float timePassed)
	{
		this->offset += this->velocity * timePassed;
	}

	inline void Render(Shader& particleShader)
	{

	}

	// static members ---------------------------------------

	inline static uint VAO = 0;
	inline static const float vertices[] = {
		// positions          // tex coords
		 1.0f, 0.0f,  1.0f,   1.0f, 0.0f,
		-1.0f, 0.0f,  1.0f,   0.0f, 0.0f,
		-1.0f, 0.0f, -1.0f,   0.0f, 1.0f,

		 1.0f, 0.0f,  1.0f,   1.0f, 0.0f,
		-1.0f, 0.0f, -1.0f,   0.0f, 1.0f,
		 1.0f, 0.0f, -1.0f,   1.0f, 1.0f
	};

	inline void InitStaticVAO()
	{
		if (VAO != 0)
			return;

		GLCall(glGenVertexArrays(1, &VAO));
		GLCall(glBindVertexArray(VAO));

		uint VBO;
		GLCall(glGenBuffers(1, &VBO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));

		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)3));

		GLCall(glBindVertexArray(0));
	}
};
