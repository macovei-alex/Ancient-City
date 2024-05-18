#pragma once

#include <vector>

#include "Shader.h"
#include "vertex.hpp"
#include "material.hpp"

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint>& indices, std::shared_ptr<Material> material) noexcept;
    Mesh(const Mesh& mesh) noexcept;
    Mesh(Mesh&& mesh) noexcept;
    inline ~Mesh() { if(VAO != 0) GLCall(glDeleteVertexArrays(1, &VAO)); }

    void Render(const Shader& shader) const;
    void DepthRender() const;

public:
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    std::shared_ptr<Material> material;

private:
    uint VAO;

    void InitBuffers();
};

