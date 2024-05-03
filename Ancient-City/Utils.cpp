#include "utils.h"

#include <format>
#include <sstream>
#include <unordered_map>

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
	return mat * glm::vec4(vec, 1.0f);
}

glm::vec3 glm::operator*(const glm::mat4& mat, const aiVector3D& vec)
{
	return mat * glm::vec4(vec.x, vec.y, vec.z, 1.0f);
}

std::ostream& operator<<(std::ostream& os, const glm::vec2& vector)
{
	return os << vector.x << ' ' << vector.y;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vector)
{
	return os << vector.x << ' ' << vector.y << ' ' << vector.z;
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

std::string_view GetKeyPressed(int key)
{
	static const std::unordered_map<int, std::string_view> keyMap =
	{
		{GLFW_KEY_UNKNOWN, "UNKNOWN"},
		{GLFW_KEY_ESCAPE, "ESCAPE"},
		{GLFW_KEY_SPACE, "SPACE"},
		{GLFW_KEY_APOSTROPHE, "APOSTROPHE"},
		{GLFW_KEY_COMMA, "+"},
		{GLFW_KEY_MINUS, "-"},
		{GLFW_KEY_PERIOD, "."},
		{GLFW_KEY_SLASH, "/"},
		{GLFW_KEY_BACKSLASH, "\\"},
		{GLFW_KEY_0, "0"},
		{GLFW_KEY_1, "1"},
		{GLFW_KEY_2, "2"},
		{GLFW_KEY_3, "3"},
		{GLFW_KEY_4, "4"},
		{GLFW_KEY_5, "5"},
		{GLFW_KEY_6, "6"},
		{GLFW_KEY_7, "7"},
		{GLFW_KEY_8, "8"},
		{GLFW_KEY_9, "9"},
		{GLFW_KEY_SEMICOLON, "SEMICOLON"},
		{GLFW_KEY_EQUAL, "EQUAL"},
		{GLFW_KEY_A, "A"},
		{GLFW_KEY_B, "B"},
		{GLFW_KEY_C, "C"},
		{GLFW_KEY_D, "D"},
		{GLFW_KEY_E, "E"},
		{GLFW_KEY_F, "F"},
		{GLFW_KEY_G, "G"},
		{GLFW_KEY_H, "H"},
		{GLFW_KEY_I, "I"},
		{GLFW_KEY_J, "J"},
		{GLFW_KEY_K, "K"},
		{GLFW_KEY_L, "L"},
		{GLFW_KEY_M, "M"},
		{GLFW_KEY_N, "N"},
		{GLFW_KEY_O, "O"},
		{GLFW_KEY_P, "P"},
		{GLFW_KEY_Q, "Q"},
		{GLFW_KEY_R, "R"},
		{GLFW_KEY_S, "S"},
		{GLFW_KEY_T, "T"},
		{GLFW_KEY_U, "U"},
		{GLFW_KEY_V, "V"},
		{GLFW_KEY_W, "W"},
		{GLFW_KEY_X, "X"},
		{GLFW_KEY_Y, "Y"},
		{GLFW_KEY_Z, "Z"},
	};

	auto it = keyMap.find(key);
	return it != keyMap.end() ? it->second : "INVALID";
}
