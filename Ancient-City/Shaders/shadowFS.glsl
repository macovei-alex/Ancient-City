#version 330 core

in VS_OUT {
	vec3 MidPosition;
	vec3 MidNormal;
	vec2 MidTexCoords;
	vec4 MidLightSpacePosition;
} fs_in;

out vec4 OutColor;

uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform vec3 ViewPosition;

uniform float AmbientStrength;
uniform float DiffuseStrength;
uniform float SpecularStrength;
uniform int SpecularExponent;

uniform sampler2D texture_diffuse1;
uniform sampler2D ShadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	vec3 projectionCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
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
	// ambient
	vec3 ambient = AmbientStrength * LightColor;
	vec3 normal = normalize(fs_in.MidNormal);

	// diffuse
	vec3 lightDirection = normalize(LightPosition - fs_in.MidPosition);
	float diffuseValue = max(dot(lightDirection, normal), 0.0);
	vec3 diffuse = DiffuseStrength * diffuseValue * LightColor;

	// specular
	vec3 viewDirection = normalize(ViewPosition - fs_in.MidPosition);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specularPower = pow(max(dot(viewDirection, reflectionDirection), 0.0), SpecularExponent);
	vec3 specular = SpecularStrength * specularPower * LightColor;

	vec4 texColor = texture(texture_diffuse1, fs_in.MidTexCoords);
	float shadow = ShadowCalculation(fs_in.MidLightSpacePosition);
	vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * texColor.rgb;

	OutColor = vec4(result, texColor.a);
}