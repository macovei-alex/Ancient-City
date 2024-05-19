#version 330 core

in vec3 MidTexCoords;

out vec4 OutColor;

uniform samplerCube DaySkyboxTexture;
uniform samplerCube NightSkyboxTexture;
uniform float MixPercent;

void main()
{
    OutColor = mix(texture(DaySkyboxTexture, MidTexCoords), texture(NightSkyboxTexture, MidTexCoords), MixPercent);
}
