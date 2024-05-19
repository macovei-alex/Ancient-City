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

float Sun::CalculateSkyboxesMixPercent() const
{
	// hours in a day
	static constexpr float firstFadeStart = 4.0f;
	static constexpr float firstFadeEnd = 6.0f;
	static constexpr float secondFadeStart = 18.0f;
	static constexpr float secondFadeEnd = 20.0f;

	float hour = CalculateHour();

	if(firstFadeStart <= hour && hour <= firstFadeEnd) // dawn
		return 1.0f - (hour - firstFadeStart) / (firstFadeEnd - firstFadeStart);

	if(secondFadeStart <= hour && hour <= secondFadeEnd) // dusk
		return (hour - secondFadeStart) / (secondFadeEnd - secondFadeStart);

	if(firstFadeEnd < hour && hour < secondFadeStart) // day
		return 0.0f;

	return 1.0f; // night
}

float Sun::CalculateHour() const
{
	float angle = glm::degrees(atan2(light.direction.y, light.direction.x));
	if(angle < 0.0f)
		angle += 360.0f;
	float hour = angle * 24.0f / 360.0f;
	return fmod(hour + 6.0f, 24.0f);
}
