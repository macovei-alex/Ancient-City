#version 330 core

in vec2 MidTexCoords;

out vec4 OutColor;

uniform sampler2D DiffuseTexture;

void main()
{
    OutColor = vec4(1.0);
}