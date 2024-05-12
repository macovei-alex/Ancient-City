#version 330 core

layout (location = 0) in vec3 InPosition;

uniform mat4 LightSpaceMatrix;
uniform mat4 ModelMatrix;

void main()
{
	gl_Position = LightSpaceMatrix * ModelMatrix * vec4(InPosition, 1.0);
}