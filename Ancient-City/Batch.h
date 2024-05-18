#pragma once

#include "utils.h"
#include "Mesh.h"

#include <memory>

class Batch
{
public:
	static std::vector<Batch*> SplitIntoBatches(const std::vector<Model*>& models);

public:
	Batch(const std::vector<Mesh*>& meshes, const std::vector<glm::mat4>& matrices);
	Batch(const Batch& other) = delete;
	Batch(Batch&& other) noexcept;
	Batch& operator=(const Batch& other) = delete;
	Batch& operator=(Batch&& other) noexcept;
	~Batch();

	void Render(const Shader& shader) const;
	void DepthRender() const;

private:
	void InitBuffers(const std::vector<Vertex>& vertices, const std::vector<uint>& indices, std::shared_ptr<Material> material);

private:
	uint VAO;
	uint indexCount;
	std::shared_ptr<Material> material;
};

