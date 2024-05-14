#include "Sun.h"

const float Sun::MODEL_POSITION_MULTIPLIER = 200.0f;

Sun::Sun(const Model& model)
	: DirectionalLightSource(),
	model(model)
{
	this->model.SetPosition(-lightDirection * MODEL_POSITION_MULTIPLIER);
}

void Sun::Rotate(float x, float y, float z)
{
	DirectionalLightSource::RotateDirection(x, y, z);
	glm::vec3 modelDirection = glm::vec3(lightDirection.x, -lightDirection.y, lightDirection.z);
	model.SetPosition(modelDirection * MODEL_POSITION_MULTIPLIER);
}

void Sun::Rotate(const glm::vec3& rotation)
{
	Rotate(rotation.x, rotation.y, rotation.z);
}

void Sun::PassTime(float time)
{
	// float angle = time * secondToHoursConversionRate * (360.0f / 24.0f);
	// Rotate(0.0f, 0.0f, angle);

	static float timePassedTotal = 0.0f;
	timePassedTotal += 2 * time;
	model.SetPosition(glm::vec3(40 * cos(timePassedTotal), 30.0f, 40 * sin(timePassedTotal)));

	glm::vec3 modelPos = model.GetPosition();
	lightDirection = glm::normalize(glm::vec3(modelPos.x, -modelPos.y, modelPos.z));
}

glm::mat4 Sun::GetLightViewMatrix() const
{
	glm::vec3 pos = model.GetPosition();
	return glm::lookAt(pos, pos + lightDirection, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Sun::SetSecondToHoursConversionRate(float nHoursPerSecond)
{
	secondToHoursConversionRate = nHoursPerSecond;
}

Model& Sun::GetModel()
{
	return model;
}

void Sun::Render(const Shader& shader) const
{
	model.Render(shader);
}
