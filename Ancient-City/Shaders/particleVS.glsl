#version 330 core

layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec2 InTexCoords;

out vec2 MidTexCoords;

uniform mat4 ProjectionMatrix;
uniform vec3 Offset;

void main()
{
	float scale = 10.0f;
	MidTexCoords = InTexCoords;

	gl_Position = ProjectionMatrix * vec4(InPosition + Offset);
}
