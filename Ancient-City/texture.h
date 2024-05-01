#pragma once

#include "Utils.h"

enum class TextureType
{
    Diffuse,
    Specular,
    Normal,
    Height
};

struct Texture
{
    uint id;
    std::string name;
    std::string path;
};
