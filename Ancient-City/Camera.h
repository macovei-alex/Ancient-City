#pragma once

#include "utils.h"
#include <glm.hpp>

class Camera
{
public:
	Camera(int width, int height, const glm::vec3& position);
	void Set(int width, int height, const glm::vec3& position);

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	glm::vec3 GetPosition() const;

	void MoveCamera(float xOffset, float yOffset, float zOffset);

	inline void MoveForward(float distance) { MoveCamera(0, 0, distance); }
	inline void MoveBackward(float distance) { MoveCamera(0, 0, -distance); }
	inline void MoveRight(float distance) { MoveCamera(distance, 0, 0); }
	inline void MoveLeft(float distance) { MoveCamera(-distance, 0, 0); }
	inline void MoveUp(float distance) { MoveCamera(0, distance, 0); }
	inline void MoveDown(float distance) { MoveCamera(0, -distance, 0); }

	void HandleMouseMovement(float xPos, float yPos);
	void HandleMouseScroll(float yOffset);

	inline void SetLastMousePos(float x, float y) { lastX = x; lastY = y; }
	inline void SetViewPort() { GLCall(glViewport(0, 0, screenWidth, screenHeight)); }

private:
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
	static const float Z_NEAR;
	static const float Z_FAR;
	static const float YAW;
	static const float PITCH;
	static const float FOV;
	static const float SPEED_FACTOR;
	static const float MOUSE_SENSITIVITY;
	static const float SPEED_BOOST_MULTIPLIER;
	static const glm::vec3 START_POSITION;
	static const glm::vec3 START_FACING;
	static const float MOUSE_SCROLL_MULTIPLIER;
};