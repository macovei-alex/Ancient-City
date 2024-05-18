#version 330 core

in vec3 MidPosition;
in vec3 MidNormal;
in vec2 MidTexCoords;
in vec4 MidLightSpacePosition;

out vec4 OutColor;

struct MaterialStruct
{
	vec3 AmbientColor;
	vec3 DiffuseColor;
	vec3 SpecularColor;
	int SpecularExponent;
};
uniform MaterialStruct Material;

uniform vec3 LightColor;
uniform vec3 LightDirection;
uniform vec3 ViewPosition;

uniform float AmbientIntensity;
uniform float DiffuseIntensity;
uniform float SpecularIntensity;

uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;
uniform sampler2D ShadowMap;

float ShadowCalculation(vec4 lightSpacePosition)
{
	vec3 projectionCoords = lightSpacePosition.xyz / lightSpacePosition.w;
	projectionCoords = projectionCoords * 0.5 + 0.5;

	if(projectionCoords.z > 1.0)
        return 0.0;

	float closestDepth = texture(ShadowMap, projectionCoords.xy).r;
    float currentDepth = projectionCoords.z;

	return currentDepth - 0.005 > closestDepth ? 1.0 : 0.0;
}

void main()
{
	// when the rasterizer is applied there is no guarantee that the normal is a unit vector
	vec3 normal = normalize(MidNormal);

	// ambient
	vec3 ambient = AmbientIntensity * Material.AmbientColor * LightColor;

	// diffuse
	float diffusePower = max(dot(LightDirection, normal), 0.0);
	vec3 diffuseTexture = texture(DiffuseTexture, MidTexCoords).rgb;
	vec3 diffuse = DiffuseIntensity * diffusePower * Material.DiffuseColor * LightColor * diffuseTexture;

	// specular
	vec3 viewDirection = normalize(ViewPosition - MidPosition);
	vec3 reflectionDirection = reflect(-LightDirection, normal);
	float specularPower = pow(max(dot(viewDirection, reflectionDirection), 0.0), Material.SpecularExponent);
	vec3 specularTexture = texture(SpecularTexture, MidTexCoords).rgb;
	vec3 specular = SpecularIntensity * specularPower * Material.SpecularColor * LightColor * specularTexture;

	// float shadow = ShadowCalculation(MidLightSpacePosition);
	// vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * texColor.rgb;
	vec3 result = ambient + diffuse + specular;

	OutColor = vec4(result, 1.0);
}