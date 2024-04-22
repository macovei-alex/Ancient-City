#pragma once

#include "Utils.h"

#include "ShaderProgram.h"
#include "texture.h"
#include "vertex.h"

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint>& indices, const std::vector<Texture>& textures);
    Mesh(uint numVertices, std::shared_ptr<Vertex> vertices, uint numIndexes, std::shared_ptr<uint> indices, const std::vector<Texture>& textures);
    void Render(ShaderProgram& shader) const;

private:
    void InitBuffers();

public:
    uint vertexCount;
    uint indexCount;

    std::shared_ptr<Vertex> vertices;
    std::shared_ptr<uint> indices;
    std::vector<Texture> textures;

    GLuint VAO;

private:
    GLuint VBO, EBO;
};

