#version 330 core

layout (location = 0) in vec3 InBufferPosition;

out vec3 MidColor;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 ParticleGeneratorPosition;
uniform vec3 ParticleOffset;
uniform vec3 ParticleColorStart;
uniform vec3 ParticleColorEnd;
uniform float ParticleColorBlendPercent;
uniform float ParticleScale;

void main()
{
	MidColor = mix(ParticleColorStart, ParticleColorEnd, ParticleColorBlendPercent);
	gl_Position = ProjectionMatrix * ViewMatrix * vec4(ParticleGeneratorPosition + ParticleScale * InBufferPosition + ParticleOffset, 1.0f);
}
