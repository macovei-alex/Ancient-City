#pragma once

#define GLEW_STATIC
#include <glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <glfw3.h>

#include <stb_image.h>

#include <assimp/types.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <format>
#include <memory>

#include "Logger.h"

#define dimof(vec) (sizeof(vec) / sizeof(vec[0])
#define LOG(message, level) Logger::cout(std::format("{}:{} - {}", fs::path(__FILE__).filename().string(), __LINE__, message), level)
#define ASSERT(cond) if (!(cond)) __debugbreak();

#define DEBUG

#ifdef DEBUG
#define GLCall(func) GLClearError(); func; ASSERT(GLLogCall(#func, __FILE__, __LINE__));
#else
#define GLCall(func) func;
#endif

namespace fs = std::filesystem;

constexpr float PI = 3.14159265359f;

namespace glm
{
	typedef glm::vec<3, unsigned int> vec3u;
	glm::vec2& operator+=(glm::vec2& vec, const aiVector2D& other);
	glm::vec3& operator+=(glm::vec3& vec, const aiVector3D& other);
	glm::vec3 operator*(const glm::mat4& mat, const glm::vec3& vec);
	glm::vec3 operator*(const glm::mat4& mat, const aiVector3D& vec);
}
typedef unsigned int uint;

enum class Layout : GLuint
{
	Location_0,
	Location_1,
	Location_2
};

enum class AtribNumber : GLuint
{
	Zero,
	One,
	Two
};

enum class Count : GLint
{
	Zero,
	One,
	Two
};

enum class StepSize : GLint
{
	Int = sizeof(int),
	UInt = sizeof(unsigned int),
	Float = sizeof(float),
};

enum class SpaceBetween : GLint
{
	Zero = 0
};

enum class StartOffset : GLint
{
	Zero = 0
};

glm::vec3& assign(glm::vec3& vec, const aiVector3D& other);
std::ostream& operator<<(std::ostream& os, const glm::vec2& vector);
std::ostream& operator<<(std::ostream& os, const glm::vec3& vector);
std::ostream& operator<<(std::ostream& os, const glm::mat4& mat);
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
