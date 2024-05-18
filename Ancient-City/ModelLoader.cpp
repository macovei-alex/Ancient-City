#include "ModelLoader.h"

#include <gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <filesystem>
namespace fs = std::filesystem;

#include "constants.h"
#include "material.hpp"

void ModelLoader::SetCurrentDirectory(const std::string& fileName)
{
	fs::path filePath = fs::absolute(fs::canonical(fileName));
	currentDirectory = filePath.remove_filename();
}

Model* ModelLoader::LoadModel(const std::string& fileName, bool smoothNormals)
{
	return LoadModel(fileName, glm::mat4(1.0f), smoothNormals);
}

Model* ModelLoader::LoadModel(const std::string& fileName, float scale)
{
	return LoadModel(fileName, glm::scale(glm::mat4(1.0f), glm::vec3(scale)), false);
}

Model* ModelLoader::LoadModel(const std::string& fileName, const glm::mat4& onLoadTransforms, bool smoothNormals)
{
	if (!defaultTexturesLoaded)
	{
		LoadDefaults();
		LOG("Default textures loaded successfully", Logger::Level::Info);
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName,
		aiProcess_Triangulate
		| aiProcess_FlipUVs
		| aiProcess_CalcTangentSpace
		| (smoothNormals ? aiProcess_GenSmoothNormals : aiProcess_GenNormals));

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG(importer.GetErrorString(), Logger::Level::Error);
		throw std::runtime_error("Failed to load model");
	}

	SetCurrentDirectory(fileName);

	Model* model = new Model();
	ProcessNode(*model, scene->mRootNode, scene, onLoadTransforms);

	LOG(std::format("Model ( {} ) loaded successfully", fileName), Logger::Level::Info);

	return model;
}

void ModelLoader::ProcessNode(Model& model, aiNode* node, const aiScene* scene, const glm::mat4& onLoadTransforms)
{
	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		model.meshes.push_back(ProcessMesh(mesh, scene, onLoadTransforms));
	}

	for (uint i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(model, node->mChildren[i], scene, onLoadTransforms);
	}
}

std::shared_ptr<Mesh> ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& onLoadTransforms)
{
	std::vector<Vertex> vertices;
	std::vector<uint> indices;

	for (uint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position += onLoadTransforms * mesh->mVertices[i];
		if (mesh->HasNormals())
		{
			vertex.normal += mesh->mNormals[i];
			vertex.normal = glm::normalize(vertex.normal);
		}
		else
		{
			vertex.normal = glm::vec3(0.0f);
		}

		if (mesh->mTextureCoords[0])
		{
			vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
		}

		/*
		auto res = glm::isnan(vertex.position);
		if (res.x || res.y || res.z)
		{
			std::cout << "|||\n" << mesh->mVertices[i] << "--\n" << onLoadTransforms << "--\n" << vertex.position << "--\n";
		}

		res = glm::isnan(vertex.normal);
		if (res.x || res.y || res.z)
		{
			std::cout << "|||\n" << vertex << "--\n";
			vertex.normal = glm::vec3(0.0f);
		}
		*/

		vertices.push_back(vertex);
	}

	for (uint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	aiMaterial* pMaterial = scene->mMaterials[mesh->mMaterialIndex];
	std::shared_ptr<Material> material = LoadMaterial(pMaterial);

	return std::make_shared<Mesh>(vertices, indices, material);
}

std::shared_ptr<Material> ModelLoader::LoadMaterial(aiMaterial* pMaterial)
{
	for(size_t i = 0; i < loadedMaterials.size(); i++)
		if (strcmp(loadedMaterials[i]->name.data(), pMaterial->GetName().C_Str()) == 0)
			return loadedMaterials[i];

	std::shared_ptr<Material> material = std::make_shared<Material>();
	material->name = pMaterial->GetName().C_Str();
	material->diffuseTexture = LoadTexture(pMaterial, aiTextureType_DIFFUSE);
	material->specularTexture = LoadTexture(pMaterial, aiTextureType_SPECULAR);
	LoadColors(pMaterial, *material);

	loadedMaterials.push_back(material);

	LOG(std::format("New material ( {} ) loaded successfully", material->name), Logger::Level::Info);

	return material;
}

void ModelLoader::LoadColors(aiMaterial* pMaterial, Material& outMaterial)
{
	aiColor4D color;

	if (pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
		outMaterial.ambientColor += color;
	else
		outMaterial.ambientColor = glm::vec3(1.0f);

	if (pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
		outMaterial.diffuseColor += color;
	else
		outMaterial.diffuseColor = glm::vec3(1.0f);

	if (pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
		outMaterial.specularColor += color;
	else
		outMaterial.specularColor = glm::vec3(1.0f);

	if(pMaterial->Get(AI_MATKEY_SPECULAR_FACTOR, outMaterial.specularExponent) != AI_SUCCESS)
		outMaterial.specularExponent = 32;
}

std::shared_ptr<Texture> ModelLoader::LoadTexture(aiMaterial* material, aiTextureType type)
{
	if (material->GetTextureCount(type) == 0)
	{
		switch (type)
		{
		case aiTextureType_DIFFUSE:
			return defaultDiffuseTexture;

		case aiTextureType_SPECULAR:
			return defaultSpecularTexture;

		default:
			LOG(std::format("Invalid texture of type ( {} ). The texture will be set to the default value of the diffuse texture", GetTypeName(type)), Logger::Level::Warning);
			return defaultDiffuseTexture;
		}
	}

	if (material->GetTextureCount(type) > 1)
		LOG(std::format("Multiple textures of type ( {} ) have been detected inside material ( {} ). Only the first texture will be loaded", GetTypeName(type), material->GetName().C_Str()), Logger::Level::Warning);

	aiString texturePath;
	material->GetTexture(type, 0, &texturePath);

	for (size_t j = 0; j < loadedTextures.size(); j++)
		if (strcmp(loadedTextures[j]->path.data(), texturePath.C_Str()) == 0)
			return loadedTextures[j];

	std::shared_ptr<Texture> texture = std::make_shared<Texture>();
	texture->id = TextureFromFile(texturePath.C_Str());
	texture->type = GetTypeName(type);
	texture->path = texturePath.C_Str();

	loadedTextures.push_back(texture);

	LOG(std::format("Texture ( {} ) loaded successfully with OpenGL ID={}", texture->path, texture->id), Logger::Level::Info);

	return texture;
}

uint ModelLoader::TextureFromFile(const std::string& fileName, bool gamma)
{
	std::string filePath = (currentDirectory / fileName).string();

	uint textureID;
	GLCall(glGenTextures(1, &textureID));

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0);

	if (data)
	{
		GLint format = GL_RGB;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		stbi_image_free(data);
	}
	else
	{
		stbi_image_free(data);
		LOG(std::format("Texture could not be loaded from {}", filePath), Logger::Level::Error);
	}

	return textureID;
}

void ModelLoader::LoadDefaults()
{
	SetCurrentDirectory(names::textures::defaults::dirPath + "\\" + names::textures::defaults::diffuse);

	defaultDiffuseTexture = std::make_shared<Texture>();
	defaultDiffuseTexture->id = TextureFromFile(names::textures::defaults::diffuse);
	defaultDiffuseTexture->type = names::textures::diffuse;
	defaultDiffuseTexture->path = names::textures::defaults::diffuse;
	loadedTextures.push_back(defaultDiffuseTexture);

	defaultSpecularTexture = std::make_shared<Texture>();
	defaultSpecularTexture->id = TextureFromFile(names::textures::defaults::specular);
	defaultSpecularTexture->type = names::textures::diffuse;
	defaultSpecularTexture->path = names::textures::defaults::specular;
	loadedTextures.push_back(defaultSpecularTexture);

	defaultMaterial = std::make_shared<Material>();
	defaultMaterial->diffuseTexture = defaultDiffuseTexture;
	defaultMaterial->specularTexture = defaultSpecularTexture;
	defaultMaterial->ambientColor = glm::vec3(1.0f);
	defaultMaterial->diffuseColor = glm::vec3(1.0f);
	defaultMaterial->specularColor = glm::vec3(1.0f);
	defaultMaterial->specularExponent = 32;
	loadedMaterials.push_back(defaultMaterial);

	defaultTexturesLoaded = true;
}
