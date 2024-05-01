#version 330 core

in vec2 MidTexCoords;

out vec4 OutColor;

uniform sampler2D Texture;

void main()
{             
    vec4 texColor = texture(Texture, MidTexCoords);
    OutColor = texColor;
}