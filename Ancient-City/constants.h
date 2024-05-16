#pragma once

#include <string>

typedef unsigned int uint;

namespace names
{
	namespace shaders
	{
		inline const std::string dirName = "Shaders";
		inline const std::string vertex = "VS.glsl";
		inline const std::string fragment = "FS.glsl";
		inline const std::string model = "model";
		inline const std::string texture = "texture";
		inline const std::string particle = "particle";
		inline const std::string skybox = "skybox";
		inline const std::string shadow = "shadow";
		inline const std::string depthMap = "depthMap";
		inline const std::string cube = "cube";
	}

	namespace textures
	{
		inline const std::string diffuse = "DiffuseTexture";
		inline const std::string specular = "SpecularTexture";
		inline const std::string normal = "NormalTexture";
		inline const std::string height = "HeightTexture";
	}

	namespace locations
	{
		constexpr char ModelMatrix[] = "ModelMatrix";
		constexpr char ViewMatrix[] = "ViewMatrix";
		constexpr char ProjectionMatrix[] = "ProjectionMatrix";
		constexpr char ViewPosition[] = "ViewPosition";
		constexpr char LightColor[] = "LightColor";
		constexpr char LightDirection[] = "LightDirection";
		constexpr char LightPosition[] = "LightPosition";
		constexpr char AmbientIntensity[] = "AmbientIntensity";
		constexpr char DiffuseIntensity[] = "DiffuseIntensity";
		constexpr char SpecularIntensity[] = "SpecularIntensity";
		constexpr char SpecularExponent[] = "SpecularExponent";
		constexpr char DiffuseTexture[] = "DiffuseTexture";
		constexpr char LightSpaceMatrix[] = "LightSpaceMatrix";
		constexpr char VP[] = "VP";
		constexpr char MVP[] = "MVP";
		constexpr char ShadowMap[] = "ShadowMap";
		constexpr char ParticleStartColor[] = "ParticleStartColor";
		constexpr char ParticleEndColor[] = "ParticleEndColor";
		constexpr char ParticleScale[] = "ParticleScale";
		constexpr char ParticlePosition[] = "ParticlePosition";
		constexpr char ParticleColorBlendPercent[] = "ParticleColorBlendPercent";
		constexpr char ParticleAlpha[] = "ParticleAlpha";
		constexpr char ObjectColor[] = "ObjectColor";
	}
}
