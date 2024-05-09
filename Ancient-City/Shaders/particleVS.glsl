#version 330 core

layout (location = 0) in vec3 InBufferPoisition;
layout (location = 1) in vec3 InColor;

out vec3 MidColor;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 ParticleGeneratorPosition;
uniform vec3 ParticleOffset;

void main()
{
	MidColor = InColor;
	gl_Position = ProjectionMatrix * ViewMatrix * vec4(ParticleGeneratorPosition + InBufferPoisition + ParticleOffset, 1);
}
