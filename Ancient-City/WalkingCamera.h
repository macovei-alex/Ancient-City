#pragma once

#include "BaseCamera.h"

class WalkingCamera : public BaseCamera
{
public:
	WalkingCamera(int width, int height, const glm::vec3& position);
	void MoveCamera(float xOffset, float yOffset, float zOffset) override;
};
