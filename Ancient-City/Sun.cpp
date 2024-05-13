#include "Sun.h"

const float Sun::MODEL_POSITION_MULTIPLIER = 1000.0f;

Sun::Sun(const Model& model)
	: model(model)
{
	this->model.SetPosition(-lightDirection * MODEL_POSITION_MULTIPLIER);
}

void Sun::Rotate(float x, float y, float z)
{
	DirectionalLightSource::RotateDirection(x, y, z);
	model.SetPosition(-lightDirection * MODEL_POSITION_MULTIPLIER);
}

void Sun::Rotate(const glm::vec3& rotation)
{
	Rotate(rotation.x, rotation.y, rotation.z);
}

void Sun::PassTime(float time)
{
	float angle = time * secondToHoursConversionRate * (360.0f / 24.0f);
	Rotate(0.0f, 0.0f, angle);
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
