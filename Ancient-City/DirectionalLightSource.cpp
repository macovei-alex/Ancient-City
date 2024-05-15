#include "DirectionalLightSource.h"

#include "utils.h"
#include "Model.h"

#include <gtc/matrix_transform.hpp>

const uint DirectionalLightSource::SHADOW_WIDTH = 1024;
const uint DirectionalLightSource::SHADOW_HEIGHT = 1024;

DirectionalLightSource::DirectionalLightSource()
	: shadowMap(DirectionalLightSource::SHADOW_WIDTH, DirectionalLightSource::SHADOW_HEIGHT)
{
	// empty
}

void DirectionalLightSource::AddAmbientStrength(float difference)
{
	float total = ambientStrength + difference;
	if (0.0f <= total && total <= 1.0f)
		ambientStrength += difference;
}

void DirectionalLightSource::AddDiffuseStrength(float difference)
{
	float total = diffuseStrength + difference;
	if (0.0f <= total && total <= 1.0f)
		diffuseStrength += difference;
}

void DirectionalLightSource::AddSpecularStrength(float difference)
{
	float total = specularStrength + difference;
	if (0.0f <= total && total <= 1.0f)
		specularStrength += difference;
}

void DirectionalLightSource::MultiplySpecularExponent(float difference)
{
	float total = specularExponent * difference;
	if (0.0f <= total && total <= 256.0f)
		specularExponent = (int)(specularExponent * difference);
}

glm::mat4 DirectionalLightSource::GetLightViewMatrix() const
{
	glm::vec3 right = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), lightDirection);
	glm::vec3 up = glm::cross(right, lightDirection);
	return glm::lookAt(lightDirection, glm::vec3(0.0f), up);
}

glm::mat4 DirectionalLightSource::GetLightProjectionMatrix() const
{
	return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 100.0f);
}

glm::mat4 DirectionalLightSource::GetLightSpaceMatrix() const
{
	return GetLightProjectionMatrix() * GetLightViewMatrix();
}

void DirectionalLightSource::RotateDirection(float x, float y, float z)
{
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(z), glm::vec3(0.0f, 0.0f, 1.0f));
	
	lightDirection = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(lightDirection, 1.0f)));
}

void DirectionalLightSource::RotateDirection(const glm::vec3& rotation)
{
	RotateDirection(rotation.x, rotation.y, rotation.z);
}

void DirectionalLightSource::WriteToShadowMap(const Shader& shadowShaders, const std::vector<Model*>& models) const
{
	glm::mat4 lightSpaceMatrix = GetLightSpaceMatrix();

	shadowShaders.Use();
	shadowShaders.SetMat4("LightSpaceMatrix", lightSpaceMatrix);

	GLCall(glViewport(0, 0, DirectionalLightSource::SHADOW_WIDTH, DirectionalLightSource::SHADOW_HEIGHT));
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));
	GLCall(glCullFace(GL_FRONT));

	shadowMap.BindForWrite();

	for (const auto& model : models)
	{
		shadowShaders.SetMat4("ModelMatrix", model->GetModelMatrix());
		model->DepthRender();
	}

	shadowMap.UnbindForWrite();
	GLCall(glCullFace(GL_BACK));
}
