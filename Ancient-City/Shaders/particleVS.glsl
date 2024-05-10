#version 330 core

layout (location = 0) in vec3 InBufferPosition;

out vec3 MidColor;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 ParticleGeneratorPosition;
uniform vec3 ParticleOffset;
uniform vec3 ParticleColor;

void main()
{
	MidColor = ParticleColor;
	gl_Position = ProjectionMatrix * ViewMatrix * vec4(ParticleGeneratorPosition + InBufferPosition + ParticleOffset, 1.0f);
}
