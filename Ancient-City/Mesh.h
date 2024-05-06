#pragma once

#include <vector>

#include "Shader.h"
#include "texture.h"
#include "VAO.h"

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint>& indices, const std::vector<Texture>& textures) noexcept;
    Mesh(const Mesh& mesh) noexcept;
    Mesh(Mesh&& mesh) noexcept;

    void Render(const Shader& shader) const;

public:
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    std::vector<Texture> textures;

private:
    VAO vao;
};

