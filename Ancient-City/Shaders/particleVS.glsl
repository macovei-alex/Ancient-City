#version 330 core

layout (location = 0) in vec3 InVertexPos;

out vec3 MidColor;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform vec3 ParticlePosition;
uniform float ParticleScale;

uniform vec3 ParticleStartColor;
uniform vec3 ParticleEndColor;
uniform float ParticleColorBlendPercent;

uniform float AmbientStrength;

void main()
{
	MidColor = AmbientStrength * mix(ParticleStartColor, ParticleEndColor, ParticleColorBlendPercent);
	gl_Position = ProjectionMatrix * ViewMatrix * vec4(ParticlePosition + ParticleScale * InVertexPos, 1.0);
}
