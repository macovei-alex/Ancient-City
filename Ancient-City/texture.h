#pragma once

#include "utils.h"

struct Texture
{
	int id;
	std::string type;
	std::string path;

	Texture() : id(-1), type(), path() {}
};
