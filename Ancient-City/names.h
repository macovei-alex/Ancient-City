#pragma once

#include <string>

namespace names
{
	namespace shaders
	{
		extern const std::string dirName = "Shaders";
		extern const std::string vertex = "VS.glsl";
		extern const std::string fragment = "FS.glsl";
		extern const std::string model = "model";
		extern const std::string lighting = "lighting";
		extern const std::string texture = "texture";
		extern const std::string particle = "particle";
		extern const std::string skybox = "skybox";
	}

	namespace textures
	{
		extern const std::string diffuse = "texture_diffuse";
		extern const std::string specular = "texture_specular";
		extern const std::string normal = "texture_normal";
		extern const std::string height = "texture_height";
	}
}
