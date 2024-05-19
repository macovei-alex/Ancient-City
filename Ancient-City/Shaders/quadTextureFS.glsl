#version 330 core

in vec2 MidTexCoords;

out vec4 OutColor;

uniform sampler2D Texture;

void main()
{             
    float depthValue = texture(Texture, MidTexCoords).r;
    OutColor = vec4(vec3(depthValue), 1.0);
}