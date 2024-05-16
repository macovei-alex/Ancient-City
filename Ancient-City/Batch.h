#pragma once

#include "utils.h"
#include "Mesh.h"

class Batch
{
public:
	static std::vector<Batch> SplitToBatches(const std::vector<Model*>& models);

public:
	Batch(const std::vector<Mesh*>& meshes, const std::vector<glm::mat4>& matrices);
	void Render(const Shader& shader) const;
	void DepthRender() const;

private:
	void InitBuffers(const std::vector<Vertex>& vertices, const std::vector<uint>& indices, const std::vector<Texture>& texture);

private:
	uint VAO;
	uint indexCount;
	std::vector<Texture> textures;
};

