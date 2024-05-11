#version 330 core

in vec3 MidColor;

out vec4 OutColor;

uniform float ParticleAlpha;

void main()
{
	OutColor = vec4(MidColor, ParticleAlpha);
}