#version 330

layout (location = 0) in vec3 MidNormal;
layout (location = 1) in vec2 MidTexCoords;

layout (location = 0) out vec4 OutColor;

uniform sampler2D texture_diffuse1;

void main()
{
	 OutColor = texture(texture_diffuse1, MidTexCoords);
}