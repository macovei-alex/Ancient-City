#version 330 core

layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec3 InNormal;
layout (location = 2) in vec2 InTexCoords;

out vec3 MidPosition;
out vec3 MidNormal;
out vec2 MidTexCoords;
out vec4 MidLightSpacePosition;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 LightSpaceMatrix;

void main()
{
    MidPosition = vec3(ModelMatrix * vec4(InPosition, 1.0));
    MidNormal = transpose(inverse(mat3(ModelMatrix))) * InNormal;
    MidTexCoords = InTexCoords;
    MidLightSpacePosition = LightSpaceMatrix * vec4(MidPosition, 1.0);

    gl_Position = ProjectionMatrix * ViewMatrix * vec4(MidPosition, 1.0);
}