#version 330 core

in vec2 MidTexCoords;

out vec4 OutColor;

uniform sampler2D ParticleTexture;

void main()
{
	OutColor = texture(ParticleTexture, MidTexCoords);
}