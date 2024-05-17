#pragma once

#include "utils.h"

struct Texture
{
	int id;
	std::string type;
	std::string path;

	Texture() : id(-1), type(), path() {}

	inline void Bind(const uint textureUnitIndex) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + textureUnitIndex));
		GLCall(glBindTexture(GL_TEXTURE_2D, id));
	}
};
