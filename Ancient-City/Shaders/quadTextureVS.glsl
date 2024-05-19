#version 330 core

layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec2 InTexCoords;

out vec2 MidTexCoords;

void main()
{
    MidTexCoords = InTexCoords;
    gl_Position = vec4(InPosition, 1.0);
}
