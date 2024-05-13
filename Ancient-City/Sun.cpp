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

void Sun::Render(const Shader& shader) const
{
	model.Render(shader);
}
