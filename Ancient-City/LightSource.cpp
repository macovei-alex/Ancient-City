#include "LightSource.h"

LightSource::LightSource(const Model& model)
	: model(model)
{
	// empty
}

LightSource::LightSource(Model&& model)
	: model(std::move(model))
{
	// empty
}

float LightSource::GetAmbientStrength() const
{
	return ambientStrength;
}

float LightSource::GetSpecularStrength() const
{
	return specularStrength;
}

float LightSource::GetDiffuseStrength() const
{
	return diffuseStrength;
}

int LightSource::GetSpecularExponent() const
{
	return specularExponent;
}

glm::vec3 LightSource::GetColor() const
{
	return lightColor;
}

void LightSource::SetAmbientStrength(float ambientStrength)
{
	if (0 > ambientStrength || ambientStrength > 1)
		return;

	this->ambientStrength = ambientStrength;
}

void LightSource::SetSpecularStrength(float specularStrength)
{
	if (0 > specularStrength || specularStrength > 1)
		return;

	this->specularStrength = specularStrength;
}

void LightSource::SetDiffuseStrength(float diffuseStrength)
{
	if (0 > diffuseStrength || diffuseStrength > 1)
		return;

	this->diffuseStrength = diffuseStrength;
}

void LightSource::SetSpecularExponent(int specularExponent)
{
	if (0 > specularExponent)
		return;

	this->specularExponent = specularExponent;
}

void LightSource::SetLightColor(const glm::vec3& lightColor)
{
	this->lightColor = lightColor;
}

void LightSource::AddAmbientStrength(float difference)
{
	SetAmbientStrength(this->ambientStrength + difference);
}

void LightSource::AddSpecularStrength(float difference)
{
	SetSpecularStrength(this->specularStrength + difference);
}

void LightSource::AddDiffuseStrength(float difference)
{
	SetDiffuseStrength(this->diffuseStrength + difference);
}

void LightSource::MultiplySpecularExponent(float difference)
{
	SetSpecularExponent(this->specularExponent * difference);
}

glm::vec3 LightSource::GetPosition() const
{
	return model.GetPosition();
}
