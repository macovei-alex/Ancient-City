#pragma once

#include "BaseCamera.h"

class FlyingCamera : public BaseCamera
{
public:
	FlyingCamera(int width, int height, const glm::vec3& position);
	void MoveCamera(float xOffset, float yOffset, float zOffset) override;
};