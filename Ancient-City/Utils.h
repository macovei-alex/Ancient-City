#pragma once

#ifdef APIENTRY
#undef APIENTRY
#endif
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <assimp/types.h>
#include <assimp/material.h>
#include <format>
#include <filesystem>

#include "Logger.h"

#define LOG(message, level) Logger::cout(std::format("{}:{} - {}", fs::path(__FILE__).filename().string(), __LINE__, message), level)
#define ASSERT(cond) if (!(cond)) __debugbreak();

#ifdef _DEBUG
#define GLCall(func) GLClearError(); func; ASSERT(GLLogCall(#func, __FILE__, __LINE__));
#else
#define GLCall(func) func;
#endif

namespace fs = std::filesystem;

glm::vec2& operator+=(glm::vec2& vec, const aiVector2D& other);
glm::vec3& operator+=(glm::vec3& vec, const aiVector3D& other);
glm::vec3& operator+=(glm::vec3& vec, const aiColor4D& other);
glm::vec3 operator*(const glm::mat4& mat, const glm::vec3& vec);
glm::vec3 operator*(const glm::mat4& mat, const aiVector3D& vec);
typedef unsigned int uint;

std::ostream& operator<<(std::ostream& os, const glm::vec2& vector);
std::ostream& operator<<(std::ostream& os, const glm::vec3& vector);
std::ostream& operator<<(std::ostream& os, const glm::mat4& mat);
std::ostream& operator<<(std::ostream & os, const aiVector3D& vec);
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
std::string_view GetKeyPressed(int key);
std::string TrimBeginEnd(const std::string& str, const std::string& beginning, const std::string& end);
glm::vec3 Rotate(const glm::vec3& vec, const glm::vec3& rotation);
std::string_view GetTypeName(aiTextureType type);

