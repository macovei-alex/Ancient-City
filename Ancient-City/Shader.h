#pragma once

#include "utils.h"
#include "Camera.h"

class Model;
class DirectionalLightSource;

class Shader
{
public:

#ifdef _DEBUG

	static inline void SetInt(const GLint location, int value)
	{
		if (location == -1)
			__debugbreak();
		GLCall(glUniform1i(location, value));
	}

	static inline void SetFloat(const GLint location, float value)
	{
		if (location == -1)
			__debugbreak();
		GLCall(glUniform1f(location, value));
	}

	static inline void SetVec3(const GLint location, const glm::vec3& value)
	{
		if (location == -1)
			__debugbreak();
		GLCall(glUniform3fv(location, 1, &value[0]));
	}
	
	static inline void SetMat4(const GLint location, const glm::mat4& mat)
	{
		if (location == -1)
			__debugbreak();
		GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]));
	}

#else

	static inline void SetInt(const GLint location, int value) { GLCall(glUniform1i(location, value)); }
	static inline void SetFloat(const GLint location, float value) { GLCall(glUniform1f(location, value)); }
	static inline void SetVec3(const GLint location, const glm::vec3& value) { GLCall(glUniform3fv(location, 1, &value[0])); }
	static inline void SetMat4(const GLint location, const glm::mat4& mat) { GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0])); }

#endif

public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	inline void Use() const { GLCall(glUseProgram(programID)); }

	inline GLuint GetID() const { return programID; }
	inline void GetLocation(const std::string& locationName) const { glGetUniformLocation(programID, locationName.c_str()); }

	inline void SetInt(const std::string& locationName, int value) const { SetInt(glGetUniformLocation(programID, locationName.c_str()), value); }
	inline void SetFloat(const std::string& locationName, float value) const { SetFloat(glGetUniformLocation(programID, locationName.c_str()), value); }
	inline void SetVec3(const std::string& locationName, const glm::vec3& value) const { SetVec3(glGetUniformLocation(programID, locationName.c_str()), value); }
	inline void SetMat4(const std::string& locationName, const glm::mat4& value) const { SetMat4(glGetUniformLocation(programID, locationName.c_str()), value);  }

	inline void SetModelMatrix(const glm::mat4& mat) const { SetMat4(ModelMatrix, mat); }
	inline void SetViewMatrix(const glm::mat4& mat) const { SetMat4(ViewMatrix, mat); }
	inline void SetProjectionMatrix(const glm::mat4& mat) const { SetMat4(ProjectionMatrix, mat); }
	inline void SetVP(const glm::mat4& mat) const { SetMat4(VP, mat); }
	inline void SetMVP(const glm::mat4& mat) const { SetMat4(MVP, mat); }
	inline void SetLightSpaceMatrix(const glm::mat4& mat) const { SetMat4(LightSpaceMatrix, mat); }
	inline void SetLightColor(const glm::vec3& vec) const { SetVec3(LightColor, vec); }
	inline void SetLightDirection(const glm::vec3& vec) const { SetVec3(LightDirection, vec); }
	inline void SetViewPosition(const glm::vec3& vec) const { SetVec3(ViewPosition, vec); }
	inline void SetAmbientIntensity(float val) const { SetFloat(AmbientIntensity, val); }
	inline void SetDiffuseIntensity(float val) const { SetFloat(DiffuseIntensity, val); }
	inline void SetSpecularIntensity(float val) const { SetFloat(SpecularIntensity, val); }
	inline void SetSpecularExponent(int val) const { SetInt(SpecularExponent, val); }
	inline void SetDiffuseTexture(int val) const { SetInt(DiffuseTexture, val); }
	inline void SetSpecularTexture(int val) const { SetInt(SpecularTexture, val); }
	inline void SetAmbientColor(const glm::vec3& vec) const { SetVec3(AmbientColor, vec); }
	inline void SetSpecularColor(const glm::vec3& vec) const { SetVec3(SpecularColor, vec); }
	inline void SetDiffuseColor(const glm::vec3& vec) const { SetVec3(DiffuseColor, vec); }
	inline void SetShadowMap(int val) const { SetInt(ShadowMap, val); }
	inline void SetParticleStartColor(const glm::vec3& vec) const { SetVec3(ParticleStartColor, vec); }
	inline void SetParticleEndColor(const glm::vec3& vec) const { SetVec3(ParticleEndColor, vec); }
	inline void SetParticleScale(float val) const { SetFloat(ParticleScale, val); }
	inline void SetParticlePosition(const glm::vec3& vec) const { SetVec3(ParticlePosition, vec); }
	inline void SetParticleBlendPercent(float val) const { SetFloat(ParticleColorBlendPercent, val); }
	inline void SetParticleAlpha(float val) const { SetFloat(ParticleAlpha, val); }
	inline void SetObjectColor(const glm::vec3& vec) const { SetVec3(ObjectColor, vec); }

private:
	bool Init(const std::string& vertexPath, const std::string& fragmentPath);
	void InitUniformLocations();
	bool CheckCompileErrors(GLuint shaderStencilTesting, const std::string& type);

private:
	GLuint programID;

	GLint ModelMatrix, ViewMatrix, ProjectionMatrix;
	GLint VP, MVP, LightSpaceMatrix;
	GLint LightColor;
	GLint LightDirection;
	GLint LightPosition;
	GLint ViewPosition;
	GLint AmbientIntensity, DiffuseIntensity, SpecularIntensity;
	GLint DiffuseTexture, SpecularTexture;
	GLint AmbientColor, DiffuseColor, SpecularColor, SpecularExponent;
	GLint ShadowMap;
	GLint ParticleStartColor, ParticleEndColor, ParticleColorBlendPercent, ParticleAlpha;
	GLint ParticleScale, ParticlePosition;
	GLint ObjectColor;
};