#version 330 core

in vec3 MidPosition;
in vec3 MidNormal;
in vec2 MidTexCoords;

out vec4 OutColor;

uniform vec3 LightColor;
uniform vec3 LightDirection;
uniform vec3 ViewPosition;

uniform float AmbientStrength;
uniform float DiffuseStrength;
uniform float SpecularStrength;
uniform int SpecularExponent;

uniform sampler2D DiffuseTexture;

void main()
{
	// when the rasterizer is applied there is no guarantee that the normal is a unit vector
	vec3 normal = normalize(MidNormal);

	// ambient
	vec3 ambient = AmbientStrength * LightColor;

	// diffuse
	float diffuseValue = max(dot(LightDirection, normal), 0.0);
	vec3 diffuse = DiffuseStrength * diffuseValue * LightColor;

	// specular
	vec3 viewDirection = normalize(ViewPosition - MidPosition);
	vec3 reflectionDirection = reflect(-LightDirection, normal);
	float specularPower = pow(max(dot(viewDirection, reflectionDirection), 0.0), SpecularExponent);
	vec3 specular = SpecularStrength * specularPower * LightColor;

	vec4 texColor = texture(DiffuseTexture, MidTexCoords);
	vec3 result = (ambient + diffuse + specular) * texColor.rgb;

	OutColor = vec4(result, texColor.a);

	// Without lighting
	// OutColor = texture(DiffuseTexture1, MidTexCoords);
}