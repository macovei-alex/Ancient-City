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
		inline const std::string quadTexture = "quadTexture";
	}

	namespace textures
	{
		inline const std::string diffuse = "DiffuseTexture";
		inline const std::string specular = "SpecularTexture";
		inline const std::string normal = "NormalTexture";
		inline const std::string height = "HeightTexture";

		namespace defaults
		{
			inline const std::string dirPath = "Models\\standalone-textures";
			inline const std::string diffuse = "default.jpg";
			inline const std::string specular = "default.jpg";
		}
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
		constexpr char DiffuseTexture[] = "DiffuseTexture";
		constexpr char SpecularTexture[] = "SpecularTexture";
		constexpr char AmbientColor[] = "Material.AmbientColor";
		constexpr char DiffuseColor[] = "Material.DiffuseColor";
		constexpr char SpecularColor[] = "Material.SpecularColor";
		constexpr char SpecularExponent[] = "Material.SpecularExponent";
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
		constexpr char DaySkyboxTexture[] = "DaySkyboxTexture";
		constexpr char NightSkyboxTexture[] = "NightSkyboxTexture";
		constexpr char MixPercent[] = "MixPercent";
	}
}

namespace values
{
	namespace textureUnits
	{
		inline uint diffuse = 0;
		inline uint specular = 1;
		inline uint shadowMap = 15;
		inline uint daySkybox = 10;
		inline uint nightSkybox = 11;
	}
}
