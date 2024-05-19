#pragma once

#include "utils.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class BaseCamera
{
public:
	BaseCamera(int width, int height, const glm::vec3& position);
	void Set(int width, int height, const glm::vec3& position);

	glm::mat4 CalculateProjectionMatrix() const;
	glm::mat4 CalculateViewMatrix() const;
	inline glm::vec3 GetPosition() const { return position; }

	inline void SetPosition(const glm::vec3& position) { this->position = position; }
	inline void SetForward(const glm::vec3& forward) { this->forward = glm::normalize(forward); }
	inline void SetLastMousePos(float x, float y) { lastX = x; lastY = y; }
	inline void SetViewPort() { glViewport(0, 0, screenWidth, screenHeight); }

	virtual void MoveCamera(float xOffset, float yOffset, float zOffset) = 0;

	inline void MoveForward(float distance) { MoveCamera(0, 0, distance); }
	inline void MoveBackward(float distance) { MoveCamera(0, 0, -distance); }
	inline void MoveRight(float distance) { MoveCamera(distance, 0, 0); }
	inline void MoveLeft(float distance) { MoveCamera(-distance, 0, 0); }
	inline void MoveUp(float distance) { MoveCamera(0, distance, 0); }
	inline void MoveDown(float distance) { MoveCamera(0, -distance, 0); }

	void HandleMouseMovement(float xPos, float yPos);
	void HandleMouseScroll(float yOffset);

protected:
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void ProcessMouseScroll(float yOffset);
	void UpdateCameraVectors();

	float zNear, zFar, fovY;
	int screenWidth, screenHeight;
	bool isPerspective;

	glm::vec3 position, forward, right, up, worldUp;

	float yaw, pitch;

	bool isFirstMouseMove = true;
	float lastX = 0.0f, lastY = 0.0f;

public:
	float speedBoostMultiplier = BaseCamera::SPEED_BOOST_MULTIPLIER;
	float speedSlowMultiplier = BaseCamera::SPEED_SLOW_MULTIPLIER;

public:
	inline static const float Z_NEAR = 0.1f;
	inline static const float Z_FAR = 2000.0f;
	inline static const float YAW = -90.0f;
	inline static const float PITCH = 0.0f;
	inline static const float FOV = 70.0f;
	inline static const float SPEED_FACTOR = 30.0f;
	inline static const float MOUSE_SENSITIVITY = 0.1f;
	inline static const float SPEED_BOOST_MULTIPLIER = 10.0f;
	inline static const float SPEED_SLOW_MULTIPLIER = 0.1f;
	inline static const glm::vec3 START_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
	inline static const glm::vec3 START_FACING = glm::vec3(0.0f, 0.0f, -1.0f);
	inline static const float MOUSE_SCROLL_MULTIPLIER = 2.0f;
};