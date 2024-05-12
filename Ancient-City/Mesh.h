#pragma once

#include <vector>

#include "Shader.h"
#include "vertex.h"
#include "texture.h"

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint>& indices, const std::vector<Texture>& textures) noexcept;
    Mesh(const Mesh& mesh) noexcept;
    Mesh(Mesh&& mesh) noexcept;
    ~Mesh();

    void Render(const Shader& shader) const;
    void DepthRender() const;

public:
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    std::vector<Texture> textures;

private:
    uint VAO;

    void InitBuffers();
};

