#pragma once

#include <string>

namespace names
{
	namespace shaders
	{
		inline const std::string dirName = "Shaders";
		inline const std::string vertex = "VS.glsl";
		inline const std::string fragment = "FS.glsl";
		inline const std::string model = "model";
		inline const std::string lighting = "lighting";
		inline const std::string texture = "texture";
		inline const std::string particle = "particle";
		inline const std::string skybox = "skybox";
	}

	namespace textures
	{
		inline const std::string diffuse = "texture_diffuse";
		inline const std::string specular = "texture_specular";
		inline const std::string normal = "texture_normal";
		inline const std::string height = "texture_height";
	}
}
