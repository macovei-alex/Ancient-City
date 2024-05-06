#version 330 core

in vec3 MidTexCoords;

out vec4 OutColor;

uniform samplerCube SkyboxTexture;

void main()
{    
    OutColor = texture(SkyboxTexture, MidTexCoords);
}
