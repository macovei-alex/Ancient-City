#include "Sun.h"

Sun::Sun(const Model& model)
	: DirectionalLightSource(),
	model(model), modelPositionMultiplier(1000.0f)
{
	this->model.SetPosition(light.direction * modelPositionMultiplier);
}

void Sun::Rotate(float x, float y, float z)
{
	DirectionalLightSource::RotateDirection(x, y, z);
	model.SetPosition(light.direction * modelPositionMultiplier);
}

void Sun::Rotate(const glm::vec3& rotation)
{
	Rotate(rotation.x, rotation.y, rotation.z);
}

void Sun::PassTime(float time)
{
	float angle = time * secondToHoursConversionRate * (360.0f / 24.0f);
	Rotate(0.0f, 0.0f, angle);

	/*
	static float timePassedTotal = 0.0f;
	timePassedTotal += 2 * time;
	model.SetPosition(glm::vec3(40 * cos(timePassedTotal), 40.0f, 40 * sin(timePassedTotal)));

	lightDirection = glm::normalize(model.GetPosition());
	*/
}
