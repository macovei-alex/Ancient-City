#include "ShadowMap.h"

const float ShadowMap::BORDER_COLOR[] = { 1.0f, 1.0f, 1.0f, 1.0f };

ShadowMap::ShadowMap(uint width, uint height)
	: width(width), height(height), textureUnitIndex(7)
{
	GLCall(glGenFramebuffers(1, &FBO));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, FBO));

	GLCall(glGenTextures(1, &textureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
	GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, ShadowMap::BORDER_COLOR));

	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureID, 0));
	GLCall(glDrawBuffer(GL_NONE));
	GLCall(glReadBuffer(GL_NONE));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG("ShadowMap framebuffer is incomplete", Logger::Level::Error);

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void ShadowMap::BindForWrite() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, FBO));
}

void ShadowMap::UnbindForWrite() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void ShadowMap::BindForRead(const Shader& shader) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + textureUnitIndex));
	GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
	shader.SetShadowMap(textureUnitIndex);
}

void ShadowMap::UnbindForRead() const
{
	GLCall(glActiveTexture(GL_TEXTURE0));
}
