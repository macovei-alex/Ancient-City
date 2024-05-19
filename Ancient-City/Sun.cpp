#include "Sun.h"

Sun::Sun(const Model& model)
	: DirectionalLightSource(),
	model(model),
	modelPositionMultiplier(1000.0f)
{
	this->model.SetPosition(light.direction * modelPositionMultiplier);
	startingDiffuseIntensity = light.diffuseIntensity;
	startingSpecularIntensity = light.specularIntensity;
}

Sun::Sun(Model&& model)
	: DirectionalLightSource(),
	model(std::move(model)),
	modelPositionMultiplier(1000.0f)
{
	this->model.SetPosition(light.direction * modelPositionMultiplier);
	startingDiffuseIntensity = light.diffuseIntensity;
	startingSpecularIntensity = light.specularIntensity;
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
	float deltaAngle = time * secondToHoursConversionRate * (360.0f / 24.0f);
	Rotate(0.0f, 0.0f, deltaAngle);
	RecalculateIntensity();

	/*
	static float timePassedTotal = 0.0f;
	timePassedTotal += 2 * time;
	model.SetPosition(glm::vec3(40 * cos(timePassedTotal), 40.0f, 40 * sin(timePassedTotal)));

	lightDirection = glm::normalize(model.GetPosition());
	*/
}

void Sun::RecalculateIntensity()
{
	static constexpr float minAngle = 180.0f / 3.0f;

	if (light.direction.y < 0.0f)
	{
		light.diffuseIntensity = 0.0f;
		light.specularIntensity = 0.0f;
		return;
	}

	float sunAngle = light.direction.y * 90.0f;
	if (sunAngle < minAngle)
	{
		light.diffuseIntensity = startingDiffuseIntensity * sunAngle / minAngle;
		light.specularIntensity = startingSpecularIntensity * sunAngle / minAngle;
	}
	else
	{
		light.diffuseIntensity = startingDiffuseIntensity;
		light.specularIntensity = startingSpecularIntensity;
	}
}
