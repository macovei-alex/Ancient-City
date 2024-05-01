#include "texture.h"

std::string TextureTypeToString(TextureType type)
{
	switch (type)
	{
	case TextureType::Diffuse:
		return "texture_diffuse";
	case TextureType::Specular:
		return "texture_specular";
	case TextureType::Normal:
		return "texture_normal";
	case TextureType::Height:
		return "texture_height";
	default:
		throw std::exception("Unknown texture type");
	}
}
