#include "Particle.h"

#include "utils.h"

Particle::Particle(const glm::vec3& velocity)
{
	Respawn(velocity);
}

void Particle::Respawn(const glm::vec3& newVelocity)
{
	position = glm::vec3(0.0f);
	velocity = newVelocity;
	lifeTime = 1.0f;
}

void Particle::Render(Shader& particleShader) const
{
	// It is assumed that the projection matrix has been set and the static data has been initialized already

	particleShader.Use();
	particleShader.SetVec3("position", position);

	GLCall(glBindVertexArray(VAO));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
	GLCall(glBindVertexArray(0));
}

void Particle::InitStaticVAO()
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
