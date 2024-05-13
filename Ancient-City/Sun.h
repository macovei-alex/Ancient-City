#pragma once

#include "DirectionalLightSource.h"
#include "Model.h"

class Sun : public DirectionalLightSource
{
public:
	Sun(const Model& model);

	void Rotate(float x, float y, float z);
	void Rotate(const glm::vec3& rotation);

	void Render(const Shader& shader) const;

public:
	static const float MODEL_POSITION_MULTIPLIER;

	Model model;
};

