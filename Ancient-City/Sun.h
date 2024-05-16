#pragma once

#include "DirectionalLightSource.h"
#include "Model.h"

class Sun : public DirectionalLightSource
{
public:
	Sun(const Model& model);

	void Rotate(float x, float y, float z);
	void Rotate(const glm::vec3& rotation);
	void PassTime(float time);

	inline void SetSecondToHoursConversionRate(float nHoursPerSecond) { secondToHoursConversionRate = nHoursPerSecond; }
	inline glm::vec3 GetPosition() const override { return model.GetPosition(); }

	inline void Render(const Shader& shader) const { model.Render(shader); }

public:
	static const float MODEL_POSITION_MULTIPLIER;

public:
	Model model;

private:
	float secondToHoursConversionRate = 1.0f;
};

