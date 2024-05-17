#pragma once

#include <vector>

#include "Shader.h"
#include "vertex.hpp"
#include "texture.hpp"

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint>& indices, const std::vector<std::shared_ptr<Texture>>& textures) noexcept;
    Mesh(const Mesh& mesh) noexcept;
    Mesh(Mesh&& mesh) noexcept;
    inline ~Mesh() { if(VAO != 0) GLCall(glDeleteVertexArrays(1, &VAO)); }

    void Render(const Shader& shader) const;
    void DepthRender() const;

public:
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    std::vector<std::shared_ptr<Texture>> textures;

private:
    uint VAO;

    void InitBuffers();
};

