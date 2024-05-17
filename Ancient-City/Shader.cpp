#include "Shader.h"

#include "DirectionalLightSource.h"
#include "Model.h"
#include "constants.h"

#include <sstream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	if (!Init(vertexPath, fragmentPath))
		programID = -1;
}

Shader::~Shader()
{
	if (programID != -1)
		GLCall(glDeleteProgram(programID));
}

bool Shader::Init(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFileStream;
	std::ifstream fShaderFileStream;

	vShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFileStream.open(vertexPath);
		fShaderFileStream.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFileStream.rdbuf();
		fShaderStream << fShaderFileStream.rdbuf();

		vShaderFileStream.close();
		fShaderFileStream.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure exception)
	{
		std::cout << "ERROR when reading the shaders: " << exception.what() << std::endl;
		throw exception;
	}

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	GLCall(glShaderSource(vertex, 1, &vShaderCode, NULL));
	GLCall(glCompileShader(vertex));
	if (CheckCompileErrors(vertex, "VERTEX"))
		return false;

	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	GLCall(glShaderSource(fragment, 1, &fShaderCode, NULL));
	GLCall(glCompileShader(fragment));
	if (CheckCompileErrors(fragment, "FRAGMENT"))
		return false;

	programID = glCreateProgram();
	GLCall(glAttachShader(programID, vertex));
	GLCall(glAttachShader(programID, fragment));
	GLCall(glLinkProgram(programID));
	if (CheckCompileErrors(programID, "PROGRAM"))
		return false;

	GLCall(glDeleteShader(vertex));
	GLCall(glDeleteShader(fragment));

	InitUniformLocations();

	return true;
}

bool Shader::CheckCompileErrors(GLuint shaderStencilTesting, const std::string& type)
{
	GLint success;
	GLchar infoLog[1024];

	if (type != "PROGRAM")
	{
		GLCall(glGetShaderiv(shaderStencilTesting, GL_COMPILE_STATUS, &success));
		if (!success)
		{
			GLCall(glGetShaderInfoLog(shaderStencilTesting, 1024, NULL, infoLog));
			std::cout << "ERROR when compiling shader of type: " << type << '\n' << infoLog << std::endl;
			return true;
		}
	}
	else
	{
		GLCall(glGetProgramiv(shaderStencilTesting, GL_LINK_STATUS, &success));
		if (!success)
		{
			GLCall(glGetProgramInfoLog(shaderStencilTesting, 1024, NULL, infoLog));
			std::cout << "ERROR when linking program of type: " << type << '\n' << infoLog << std::endl;
			return true;
		}
	}

	return false;
}

void Shader::InitUniformLocations()
{
	ModelMatrix = glGetUniformLocation(programID, names::locations::ModelMatrix);
	ViewMatrix = glGetUniformLocation(programID, names::locations::ViewMatrix);
	ProjectionMatrix = glGetUniformLocation(programID, names::locations::ProjectionMatrix);
	VP = glGetUniformLocation(programID, names::locations::VP);
	MVP = glGetUniformLocation(programID, names::locations::MVP);
	LightSpaceMatrix = glGetUniformLocation(programID, names::locations::LightSpaceMatrix);
	LightColor = glGetUniformLocation(programID, names::locations::LightColor);
	LightDirection = glGetUniformLocation(programID, names::locations::LightDirection);
	LightPosition = glGetUniformLocation(programID, names::locations::LightPosition);
	ViewPosition = glGetUniformLocation(programID, names::locations::ViewPosition);
	AmbientIntensity = glGetUniformLocation(programID, names::locations::AmbientIntensity);
	DiffuseIntensity = glGetUniformLocation(programID, names::locations::DiffuseIntensity);
	SpecularIntensity = glGetUniformLocation(programID, names::locations::SpecularIntensity);
	DiffuseTexture = glGetUniformLocation(programID, names::locations::DiffuseTexture);
	SpecularTexture = glGetUniformLocation(programID, names::locations::SpecularTexture);
	AmbientColor = glGetUniformLocation(programID, names::locations::AmbientColor);
	DiffuseColor = glGetUniformLocation(programID, names::locations::DiffuseColor);
	SpecularColor = glGetUniformLocation(programID, names::locations::SpecularColor);
	SpecularExponent = glGetUniformLocation(programID, names::locations::SpecularExponent);
	ShadowMap = glGetUniformLocation(programID, names::locations::ShadowMap);
	ParticleStartColor = glGetUniformLocation(programID, names::locations::ParticleStartColor);
	ParticleEndColor = glGetUniformLocation(programID, names::locations::ParticleEndColor);
	ParticleScale = glGetUniformLocation(programID, names::locations::ParticleScale);
	ParticlePosition = glGetUniformLocation(programID, names::locations::ParticlePosition);
	ParticleColorBlendPercent = glGetUniformLocation(programID, names::locations::ParticleColorBlendPercent);
	ParticleAlpha = glGetUniformLocation(programID, names::locations::ParticleAlpha);
	ObjectColor = glGetUniformLocation(programID, names::locations::ObjectColor);
}
