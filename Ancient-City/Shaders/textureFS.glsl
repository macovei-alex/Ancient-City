#version 330

in vec3 MidPosition;
in vec3 MidNormal;
in vec2 MidTextureCoords;

out vec4 OutColor;

uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform vec3 ViewPosition;

uniform float AmbientStrength;
uniform float DiffuseStrength;
uniform float SpecularStrength;
uniform int SpecularExponent;

uniform sampler2D texture_diffuse1;

void main()
{
	// With lighting
	
	vec3 ambient = AmbientStrength * LightColor;
	vec3 normal = normalize(MidNormal);

	vec3 lightDirection = normalize(LightPosition - MidPosition);
	float diffuseValue = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = DiffuseStrength * diffuseValue * LightColor;

	vec3 viewDirection = normalize(ViewPosition - MidPosition);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specularPower = pow(max(dot(viewDirection, reflectionDirection), 0.0), SpecularExponent);
	vec3 specular = SpecularStrength * specularPower * LightColor;

	vec4 textureColor = texture(texture_diffuse1, MidTextureCoords);
	vec3 result = (ambient + diffuse + specular) * textureColor.rgb;

	OutColor = vec4(result, textureColor.a);


	// Without lighting

	// OutColor = texture(texture_diffuse1, MidTexCoords);
}