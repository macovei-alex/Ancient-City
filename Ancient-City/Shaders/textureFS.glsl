#version 330

in vec3 MidNormal;
in vec2 MidTexCoords;

out vec4 OutColor;

uniform sampler2D texture_diffuse1;

void main()
{
	 OutColor = texture(texture_diffuse1, MidTexCoords);
}