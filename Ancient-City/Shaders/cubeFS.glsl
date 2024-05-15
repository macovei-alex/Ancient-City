#version 330 core

in vec3 MidPosition;
in vec3 MidNormal;
in vec4 MidLightSpacePosition;

out vec4 OutColor;

uniform vec3 LightColor;
uniform vec3 LightDirection;
uniform vec3 ViewPosition;

uniform float AmbientStrength;
uniform float DiffuseStrength;
uniform float SpecularStrength;
uniform int SpecularExponent;
uniform vec3 ObjectColor;

uniform sampler2D ShadowMap;

float ShadowCalculation(vec4 lightSpacePosition)
{
	vec3 projectionCoords = lightSpacePosition.xyz / lightSpacePosition.w;
	projectionCoords = projectionCoords * 0.5 + 0.5;

	if(projectionCoords.z > 1.0)
        return 0.0;

	float closestDepth = texture(ShadowMap, projectionCoords.xy).r;
    float currentDepth = projectionCoords.z;

	float shadow = currentDepth - 0.005 > closestDepth ? 1.0 : 0.0;
	return shadow;
}

void main()
{
	vec3 normal = normalize(MidNormal);

	// ambient
	vec3 ambient = AmbientStrength * LightColor;

	// diffuse
	float diffuseValue = max(dot(normal, LightDirection), 0.0);
	vec3 diffuse = DiffuseStrength * diffuseValue * LightColor;

	// specular
	vec3 viewDirection = normalize(ViewPosition - MidPosition);
	vec3 reflectionDirection = reflect(-LightDirection, normal);
	float specularPower = pow(max(dot(viewDirection, reflectionDirection), 0.0), SpecularExponent);
	vec3 specular = SpecularStrength * specularPower * LightColor;

	// float shadow = ShadowCalculation(MidLightSpacePosition);
	// vec3 result = ambient + (1.0 - shadow) * (diffuse + specular);
	vec3 result = (ambient + diffuse + specular) * ObjectColor;

	OutColor = vec4(result, 1.0);
}