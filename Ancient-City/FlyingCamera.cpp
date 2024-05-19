#include "FlyingCamera.h"

FlyingCamera::FlyingCamera(int width, int height, const glm::vec3& position)
	: BaseCamera(width, height, position)
{
	speedBoostMultiplier = BaseCamera::SPEED_BOOST_MULTIPLIER;
	speedSlowMultiplier = BaseCamera::SPEED_SLOW_MULTIPLIER;
}

void FlyingCamera::MoveCamera(float xOffset, float yOffset, float zOffset)
{
	position += xOffset * right * BaseCamera::SPEED_FACTOR;
	position += yOffset * worldUp * BaseCamera::SPEED_FACTOR;
	position += zOffset * forward * BaseCamera::SPEED_FACTOR;
}