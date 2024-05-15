#include "Camera.h"

#include <gtc/matrix_transform.hpp>

const float Camera::Z_NEAR = 0.1f;
const float Camera::Z_FAR = 2000.0f;
const float Camera::YAW = -90.0f;
const float Camera::PITCH = 0.0f;
const float Camera::FOV = 70.0f;
const float Camera::SPEED_FACTOR = 20.0f;
const float Camera::MOUSE_SENSITIVITY = 0.1f;
const float Camera::SPEED_BOOST_MULTIPLIER = 10.0f;
const float Camera::SPEED_SLOW_MULTIPLIER = 0.1f;
const glm::vec3 Camera::START_POSITION = glm::vec3(0.0f, 0.0f, 10.0f);
const glm::vec3 Camera::START_FACING = glm::vec3(0.0f, 0.0f, -1.0f);
const float Camera::MOUSE_SCROLL_MULTIPLIER = 2.0f;

Camera::Camera(int width, int height, const glm::vec3& position)
{
	lastX = (float)INT_MAX;
	lastY = (float)INT_MAX;
	Set(width, height, position);
}

void Camera::Set(int width, int height, const glm::vec3& position)
{
	if (width == 0 || height == 0)
	{
		LOG("Camera width or height is 0", Logger::Level::Warning);
		return;
	}

	isPerspective = true;
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	yaw = Camera::YAW;
	pitch = Camera::PITCH;
	fovY = Camera::FOV;
	zNear = Camera::Z_NEAR;
	zFar = Camera::Z_FAR;

	this->screenWidth = width;
	this->screenHeight = height;
	this->position = position;

	if (lastX == INT_MAX && lastY == INT_MAX)
	{
		lastX = width / 2.0f;
		lastY = height / 2.0f;
	}
	isFirstMouseMove = true;

	SetViewPort();
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(position, position + forward, up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	if (isPerspective)
	{
		float aspectRatio = (float)screenWidth / screenHeight;
		return glm::perspective(glm::radians(fovY), aspectRatio, zNear, zFar);
	}

	static const float scaleFactor = 10.0f;
	return glm::ortho(
		-screenWidth / scaleFactor, screenWidth / scaleFactor,
		-screenHeight / scaleFactor, screenHeight / scaleFactor, 
		-zFar, zFar);
}

glm::vec3 Camera::GetPosition() const
{
	return position;
}

void Camera::MoveCamera(float xOffset, float yOffset, float zOffset)
{
	static const glm::vec3 straightUp = glm::vec3(0.0f, 1.0f, 0.0f);

	position += xOffset * right * Camera::SPEED_FACTOR;
	position += yOffset * straightUp * Camera::SPEED_FACTOR;
	position += zOffset * forward * Camera::SPEED_FACTOR;
}

void Camera::HandleMouseMovement(float deltaX, float deltaY)
{
	ProcessMouseMovement((deltaX - lastX) * MOUSE_SENSITIVITY, (lastY - deltaY) * MOUSE_SENSITIVITY);
	SetLastMousePos(deltaX, deltaY);
	UpdateCameraVectors();
}

void Camera::HandleMouseScroll(float yOffset)
{
	ProcessMouseScroll(yOffset);
	UpdateCameraVectors();
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
	if (isFirstMouseMove)
	{
		isFirstMouseMove = false;
		return;
	}

	yaw += xOffset;
	pitch += yOffset;

	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		else if (pitch < -89.0f)
			pitch = -89.0f;
	}
}

void Camera::ProcessMouseScroll(float yOffset)
{
	fovY -= Camera::MOUSE_SCROLL_MULTIPLIER * yOffset;

	if (fovY <= 1.0f)
		fovY = 1.0f;
	else if (fovY >= 70.0f)
		fovY = 70.0f;
}

void Camera::UpdateCameraVectors()
{
	forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	forward = glm::normalize(forward);
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}