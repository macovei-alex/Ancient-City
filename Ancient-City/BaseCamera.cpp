#include "BaseCamera.h"

BaseCamera::BaseCamera(int width, int height, const glm::vec3& position)
{
	lastX = (float)INT_MAX;
	lastY = (float)INT_MAX;
	Set(width, height, position);
}

void BaseCamera::Set(int width, int height, const glm::vec3& position)
{
	if (width == 0 || height == 0)
	{
		LOG("Camera width or height is 0", Logger::Level::Warning);
		return;
	}

	isPerspective = true;
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	yaw = BaseCamera::YAW;
	pitch = BaseCamera::PITCH;
	fovY = BaseCamera::FOV;
	zNear = BaseCamera::Z_NEAR;
	zFar = BaseCamera::Z_FAR;

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

glm::mat4 BaseCamera::CalculateProjectionMatrix() const
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
		zNear, zFar);
}

glm::mat4 BaseCamera::CalculateViewMatrix() const
{
	return glm::lookAt(position, position + forward, up);
}

void BaseCamera::HandleMouseMovement(float deltaX, float deltaY)
{
	ProcessMouseMovement((deltaX - lastX) * MOUSE_SENSITIVITY, (lastY - deltaY) * MOUSE_SENSITIVITY);
	SetLastMousePos(deltaX, deltaY);
	UpdateCameraVectors();
}

void BaseCamera::HandleMouseScroll(float yOffset)
{
	ProcessMouseScroll(yOffset);
	UpdateCameraVectors();
}

void BaseCamera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
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

void BaseCamera::ProcessMouseScroll(float yOffset)
{
	fovY -= BaseCamera::MOUSE_SCROLL_MULTIPLIER * yOffset;

	if (fovY <= 1.0f)
		fovY = 1.0f;
	else if (fovY >= 70.0f)
		fovY = 70.0f;
}

void BaseCamera::UpdateCameraVectors()
{
	forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	forward = glm::normalize(forward);
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}