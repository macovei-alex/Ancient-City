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

private:
    static void ProcessNode(Model& model, aiNode* node, const aiScene* scene);
    static Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    static std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName);
    static GLuint TextureFromFile(const std::string& fileName, bool gamma = true);
    static void SetCurrentDirectory(const std::string& fileName);

private:
    static std::vector<Texture> loadedTextures;
    static fs::path currentDirectory;
};
