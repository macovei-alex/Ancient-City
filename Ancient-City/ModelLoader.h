#pragma once

#include "Utils.h"

#include "Model.h"
#include "Mesh.h"
#include "texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static class ModelLoader
{
public:
    static Model LoadModel(const std::string& path, bool smoothNormals = false);
    static Model LoadModel(const std::string& path, bool smoothNormals, const glm::vec3& scale, const glm::vec3& rotate);

private:
    static void ProcessNode(Model& model, aiNode* node, const aiScene* scene, const glm::vec3& scale, const glm::vec3& rotation);
    static Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const glm::vec3& scale, const glm::vec3& rotation);
    static std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& textureName);
    static GLuint TextureFromFile(const std::string& fileName, bool gamma = true);
    static void SetCurrentDirectory(const std::string& fileName);

private:
    static std::vector<Texture> loadedTextures;
    static fs::path currentDirectory;
};
