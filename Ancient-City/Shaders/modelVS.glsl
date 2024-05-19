#version 330 core

layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec3 InNormal;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(InPosition, 1.0f);
}