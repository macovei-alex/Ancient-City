#version 330 core

layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec3 InNormal;
layout (location = 2) in vec2 InTexCoords;

out VS_OUT {
    vec3 MidPosition;
    vec3 MidNormal;
    vec2 MidTexCoords;
    vec4 MidLightSpacePosition;
} vs_out;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 LightSpaceMatrix;

void main()
{
    vs_out.MidPosition = vec3(ModelMatrix * vec4(InPosition, 1.0));
    vs_out.MidNormal = transpose(inverse(mat3(ModelMatrix))) * InNormal;
    vs_out.MidTexCoords = InTexCoords;
    vs_out.MidLightSpacePosition = LightSpaceMatrix * vec4(vs_out.MidPosition, 1.0);

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(InPosition, 1.0);
}