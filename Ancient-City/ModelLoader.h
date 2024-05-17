#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.h"

#include <vector>

class ModelLoader
{
	ModelLoader() = delete;

public:
	static Model* LoadModel(const std::string& path, bool smoothNormals = false);
	static Model* LoadModel(const std::string& path, float scale);
	static Model* LoadModel(const std::string& path, const glm::mat4& onLoadTransforms, bool smoothNormals = false);

private:
	static void ProcessNode(Model& model, aiNode* node, const aiScene* scene, const glm::mat4& onLoadTransforms);
	static std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& onLoadTransforms);
	static std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& textureName);
	static GLuint TextureFromFile(const std::string& fileName, bool gamma = true);
	static void SetCurrentDirectory(const std::string& fileName);
	static void LoadDefaultTextures();

private:
	inline static std::vector<std::shared_ptr<Texture>> loadedTextures = {};
	inline static fs::path currentDirectory = "";
	inline static bool defaultTexturesLoaded = false;
	inline static std::shared_ptr<Texture> defaultDiffuseTexture = nullptr;
};
