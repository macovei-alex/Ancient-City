#version 330 core

layout (location = 0) in vec3 InPosition;

out vec3 MidTexCoords;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

void main()
{
    MidTexCoords = vec3(InPosition.x, InPosition.y, -InPosition.z);

    vec4 pos = ProjectionMatrix * ViewMatrix * vec4(InPosition, 1.0f);
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
}