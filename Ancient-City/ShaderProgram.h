#pragma once

#include "utils.h"

class ShaderProgram
{
public:
	ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
	~ShaderProgram();

	void Use() const;

	inline GLuint GetID() const { return ID; }

	void SetInt(const std::string& locationName, int value) const;
	void SetFloat(const std::string& locationName, float value) const;
	void SetVec3(const std::string& locationName, const glm::vec3& value) const;
	void SetMat4(const std::string& locationName, const glm::mat4& mat) const;

private:
	bool Init(const std::string& vertexPath, const std::string& fragmentPath);
	bool CheckCompileErrors(GLuint shaderStencilTesting, const std::string& type);

private:
	GLuint ID;

	GLuint modelMatrixLocation;
	GLuint viewMatrixLocation;
	GLuint projectionMatrixLocation;
};