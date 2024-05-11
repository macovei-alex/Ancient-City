#include "LightSource.h"

void LightSource::AddAmbientStrength(float difference)
{
	if(IsBetween(this->ambientStrength + difference, 0.0f, 1.0f))
		this->ambientStrength += difference;
}

void LightSource::AddSpecularStrength(float difference)
{
	if(IsBetween(this->specularStrength + difference, 0.0f, 1.0f))
		this->specularStrength += difference;
}

void LightSource::AddDiffuseStrength(float difference)
{
	if (IsBetween(this->diffuseStrength + difference, 0.0f, 1.0f))
		this->diffuseStrength += difference;
}

void LightSource::MultiplySpecularExponent(float difference)
{
	if (IsBetween(this->specularExponent * difference, 0.0f, 256.0f))
		this->specularExponent = int(this->specularExponent * difference);
}
