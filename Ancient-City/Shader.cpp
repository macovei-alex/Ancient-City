#include "Shader.h"

#include <sstream>

#include "LightSource.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	if (!Init(vertexPath, fragmentPath))
		ID = -1;
}

Shader::~Shader()
{
	if (ID != -1)
		GLCall(glDeleteProgram(ID));
}

void Shader::Use() const
{
	GLCall(glUseProgram(ID));
}

void Shader::SetInt(const std::string& locationName, int value) const
{
	GLCall(glUniform1i(glGetUniformLocation(ID, locationName.c_str()), value));
}

void Shader::SetFloat(const std::string& locationName, float value) const
{
	GLCall(glUniform1f(glGetUniformLocation(ID, locationName.c_str()), value));
}

void Shader::SetVec3(const std::string& locationName, const glm::vec3& value) const
{
	GLCall(glUniform3fv(glGetUniformLocation(ID, locationName.c_str()), 1, &value[0]));
}

void Shader::SetMat4(const std::string& locationName, const glm::mat4& mat) const
{
	GLCall(glUniformMatrix4fv(glGetUniformLocation(ID, locationName.c_str()), 1, GL_FALSE, &mat[0][0]));
}

void Shader::SetUniforms(Camera* camera, LightSource* lightSource, Model* model, uint bits) const
{
	if (bits & Uniforms::LightColor)
		SetVec3("LightColor", lightSource->GetColor());
	if (bits & Uniforms::LightPosition)
		SetVec3("LightPosition", lightSource->GetPosition());
	if (bits & Uniforms::ViewPosition)
		SetVec3("ViewPosition", camera->GetPosition());

	if (bits & Uniforms::AmbientStrength)
		SetFloat("AmbientStrength", lightSource->GetAmbientStrength());
	if (bits & Uniforms::DiffuseStrength)
		SetFloat("DiffuseStrength", lightSource->GetDiffuseStrength());
	if (bits & Uniforms::SpecularStrength)
		SetFloat("SpecularStrength", lightSource->GetSpecularStrength());
	if (bits & Uniforms::SpecularExponent)
		SetInt("SpecularExponent", lightSource->GetSpecularExponent());

	if (bits & Uniforms::ViewMatrix)
		SetMat4("ViewMatrix", camera->GetViewMatrix());
	if (bits & Uniforms::ProjectionMatrix)
		SetMat4("ProjectionMatrix", camera->GetProjectionMatrix());
	if(bits & Uniforms::ModelMatrix)
		SetMat4("ModelMatrix", model->GetModelMatrix());
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
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	if (CheckCompileErrors(vertex, "VERTEX"))
		return false;

	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	if (CheckCompileErrors(fragment, "FRAGMENT"))
		return false;

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	if (CheckCompileErrors(ID, "PROGRAM"))
		return false;

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return true;
}

bool Shader::CheckCompileErrors(GLuint shaderStencilTesting, const std::string& type)
{
	GLint success;
	GLchar infoLog[1024];

	if (type != "PROGRAM")
	{
		glGetShaderiv(shaderStencilTesting, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderStencilTesting, 1024, NULL, infoLog);
			std::cout << "ERROR when compiling shader of type: " << type << '\n' << infoLog << "\n---------------------------------------------------\n";
			return true;
		}
	}
	else
	{
		glGetProgramiv(shaderStencilTesting, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderStencilTesting, 1024, NULL, infoLog);
			std::cout << "ERROR when linking program of type: " << type << '\n' << infoLog << "\n---------------------------------------------------\n";
			return true;
		}
	}

	return false;
}