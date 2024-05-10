#version 330 core

layout (location = 0) in vec3 InBufferPoisition;

out vec3 MidColor;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 ParticleGeneratorPosition;
uniform vec3 ParticleOffset;

void main()
{
	MidColor = vec3(1.0f, 1.0f, 1.0f);
	gl_Position = ProjectionMatrix * ViewMatrix * vec4(ParticleGeneratorPosition + InBufferPoisition + ParticleOffset, 1.0f);
}
