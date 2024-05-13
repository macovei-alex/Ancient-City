#include "LightSource.h"

LightSource::LightSource(const Model& model) 
	: model(model)
{
	InitShadowMap();
}

void LightSource::AddAmbientStrength(float difference)
{
	float total = ambientStrength + difference;
	if(0.0f <= total && total <= 1.0f)
		ambientStrength += difference;
}

void LightSource::AddDiffuseStrength(float difference)
{
	float total = diffuseStrength + difference;
	if (0.0f <= total && total <= 1.0f)
		diffuseStrength += difference;
}

void LightSource::AddSpecularStrength(float difference)
{
	float total = specularStrength + difference;
	if (0.0f <= total && total <= 1.0f)
		specularStrength += difference;
}

void LightSource::MultiplySpecularExponent(float difference)
{
	float total = specularExponent * difference;
	if (1.0f <= total && total <= 256.0f)
		this->specularExponent = (int)total;
}

void LightSource::InitShadowMap()
{
	GLCall(glGenFramebuffers(1, &depthMapFBO));
	GLCall(glGenTextures(1, &depthMap));

	GLCall(glActiveTexture(GL_TEXTURE15));
	GLCall(glBindTexture(GL_TEXTURE_2D, depthMap));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, LightSource::SHADOW_WIDTH, LightSource::SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

	static const float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0));

	GLCall(glDrawBuffer(GL_NONE));
	GLCall(glReadBuffer(GL_NONE));

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glActiveTexture(GL_TEXTURE0));
}

void LightSource::CreateShadowMap(Shader& shadowShaders, const std::vector<Model*>& models) const
{
	glm::mat4 lightSpaceMatrix = GetLightSpaceMatrix();

	shadowShaders.Use();
	shadowShaders.SetMat4("LightSpaceMatrix", lightSpaceMatrix);

	GLCall(glViewport(0, 0, LightSource::SHADOW_WIDTH, LightSource::SHADOW_HEIGHT));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO));
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));
	GLCall(glCullFace(GL_FRONT));

	for (const auto& model : models)
	{
		shadowShaders.SetMat4("ModelMatrix", model->GetModelMatrix());
		model->DepthRender();
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glCullFace(GL_BACK));
}
