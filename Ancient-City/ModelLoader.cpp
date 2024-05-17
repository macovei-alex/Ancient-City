#include "ModelLoader.h"

#include <gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <filesystem>
namespace fs = std::filesystem;

#include "constants.h"

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
		LoadDefaultTextures();
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

	LOG(std::format("Model {} loaded successfully", fileName), Logger::Level::Info);

	return model;
}

void ModelLoader::ProcessNode(Model& model, aiNode* node, const aiScene* scene, const glm::mat4& onLoadTransforms)
{
	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		model.meshes->push_back(ProcessMesh(mesh, scene, onLoadTransforms));
	}

	for (uint i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(model, node->mChildren[i], scene, onLoadTransforms);
	}
}

Mesh ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& onLoadTransforms)
{
	std::vector<Vertex> vertices;
	std::vector<uint> indices;
	std::vector<Texture> textures;

	for (uint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position += onLoadTransforms * mesh->mVertices[i];
		if (mesh->HasNormals())
		{
			vertex.normal += mesh->mNormals[i];
			vertex.normal = glm::normalize(vertex.normal);
		}

		if (mesh->mTextureCoords[0])
		{
			vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
		}

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

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, names::textures::diffuse);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, names::textures::specular);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, names::textures::height);
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, names::textures::height);
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> ModelLoader::LoadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& textureName)
{
	std::vector<Texture> textures;
	for (uint i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString texturePath;
		material->GetTexture(type, i, &texturePath);

		bool wasLoaded = false;
		for (size_t j = 0; j < loadedTextures.size(); j++)
		{
			if (strcmp(loadedTextures[j].path.data(), texturePath.C_Str()) == 0)
			{
				textures.push_back(loadedTextures[j]);
				wasLoaded = true;
				break;
			}
		}

		if (!wasLoaded)
		{
			Texture texture;
			texture.id = TextureFromFile(texturePath.C_Str());
			texture.type = textureName;
			texture.path = texturePath.C_Str();

			LOG(std::format("Texture {} loaded successfully with OpenGL ID={}", texture.path, texture.id), Logger::Level::Info);

			textures.push_back(texture);
			loadedTextures.push_back(texture);
		}
	}
	return textures;
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

void ModelLoader::LoadDefaultTextures()
{
	SetCurrentDirectory("Models\\standalone-textures\\default.jpg");

	defaultDiffuseTexture.id = TextureFromFile("default.jpg");
	defaultDiffuseTexture.type = names::textures::diffuse;
	defaultDiffuseTexture.path = "default.jpg";

	defaultDiffuseTexture;
	defaultTexturesLoaded = true;
}
