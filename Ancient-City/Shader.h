#pragma once

#include <bitset>

#include "utils.h"
#include "Camera.h"

class Model;
class LightSource;

class Shader
{
public:
	enum Uniforms : uint
	{
		LightColor =			0b1,
		LightPosition =			0b10,
		LightDirection =		0b100,
		ViewPosition = 			0b1000,
		AmbientStrength =		0b10000,
		DiffuseStrength =		0b100000,
		SpecularStrength =		0b1000000,
		SpecularExponent =		0b10000000,
		ModelMatrix =			0b100000000,
		ViewMatrix =			0b1000000000,
		ProjectionMatrix =		0b10000000000,
		TextureDiffuse =		0b100000000000,

		DefaultOptions = LightColor | LightPosition | ViewPosition | AmbientStrength | DiffuseStrength | SpecularStrength | SpecularExponent | ViewMatrix | ProjectionMatrix,
	};

public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void Use() const;

	inline GLuint GetID() const { return ID; }

	void SetInt(const std::string& locationName, int value) const;
	void SetFloat(const std::string& locationName, float value) const;
	void SetVec3(const std::string& locationName, const glm::vec3& value) const;
	void SetMat4(const std::string& locationName, const glm::mat4& mat) const;
	void SetUniforms(Camera* camera, LightSource* lightSource, Model* model, uint bits) const;

private:
	bool Init(const std::string& vertexPath, const std::string& fragmentPath);
	bool CheckCompileErrors(GLuint shaderStencilTesting, const std::string& type);

private:
	GLuint ID;

	GLuint modelMatrixLocation;
	GLuint viewMatrixLocation;
	GLuint projectionMatrixLocation;
};