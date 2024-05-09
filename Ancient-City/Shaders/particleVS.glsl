#version 330 core

layout (location = 0) in vec3 InBasePosition;
layout (location = 1) in vec3 InBaseColor;

out vec3 MidColor;

uniform mat4 ProjectionMatrix;
uniform vec3 ParticlePosition;

void main()
{
	MidColor = InBaseColor;
	gl_Position = ProjectionMatrix * vec4(InBasePosition + ParticlePosition, 1);
}
