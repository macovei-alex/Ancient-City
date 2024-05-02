#include "utils.h"

#include <format>
#include <sstream>

glm::vec2& glm::operator+=(glm::vec2& vec, const aiVector2D& other)
{
	vec.x += other.x;
	vec.y += other.y;
	return vec;
}

glm::vec3& glm::operator+=(glm::vec3& vec, const aiVector3D& other)
{
	vec.x += other.x;
	vec.y += other.y;
	vec.z += other.z;
	return vec;
}

glm::vec3 glm::operator*(const glm::mat4& mat, const glm::vec3& vec)
{
	glm::vec3 result = mat * glm::vec4(vec, 1.0f);
	return glm::vec3(result);
}

glm::vec3 glm::operator*(const glm::mat4& mat, const aiVector3D& vec)
{
	glm::vec3 result = mat * glm::vec4(vec.x, vec.y, vec.z, 1.0f);
	return glm::vec3(result);
}

std::ostream& operator<<(std::ostream& os, const glm::vec2& vector)
{
	os << vector.x << ' ' << vector.y;
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vector)
{
	os << vector.x << ' ' << vector.y << ' ' << vector.z;
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::mat4& mat)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			os << mat[j][i] << ' ';
		os << '\n';
	}
	return os;
}

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	GLenum error;
	bool ret = true;
	while ((error = glGetError()) != GL_NO_ERROR)
	{
		std::stringstream ss;
		ss << std::hex << error;
		std::string errorHex = ss.str();

		std::cout << std::format("[OpenGL Error]:\n\t code ( 0x{} )\n\t in ( {} )\n\t at ( line {} )\n\t during ( {} )\n", errorHex, file, line, function);
		ret = false;
	}
	return ret;
}
