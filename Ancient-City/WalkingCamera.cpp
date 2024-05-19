#include "WalkingCamera.h"

WalkingCamera::WalkingCamera(int width, int height, const glm::vec3& position)
	: BaseCamera(width, height, position)
{
		speedBoostMultiplier = 2.5f;
		speedSlowMultiplier = 0.25f;
}

void WalkingCamera::MoveCamera(float xOffset, float yOffset, float zOffset)
{
	glm::vec3 straightRight = right;
	straightRight.y = 0.0f;

	glm::vec3 straightForward = forward;
	straightForward.y = 0.0f;
	straightForward = glm::normalize(straightForward);

	position += xOffset * straightRight * BaseCamera::SPEED_FACTOR;
	position += zOffset * straightForward * BaseCamera::SPEED_FACTOR;
}
