#pragma once

#include <vector>

#include "ShaderProgram.h"
#include "texture.h"
#include "vertex.h"

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint>& indices, const std::vector<Texture>& textures);
    Mesh(const Mesh& mesh);
    Mesh(Mesh&& mesh);

    void Render(const ShaderProgram& shader) const;

private:
    void InitBuffers();

public:
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    std::vector<Texture> textures;

private:
    GLuint VAO, VBO, EBO;
};

