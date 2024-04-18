// Example 1: Load and Print
//
// Load the data from the .obj then print it into a file
//	called e1Out.txt

// Iostream - STD I/O Library
#include <iostream>

// fStream - STD File I/O Library
#include <fstream>

// OBJ_Loader - .obj Loader
#include "OBJ_Loader.h"
#include "Model.h"

// Main function
Model* ObjLoaderMain(const std::string& fileName)
{
	objl::Loader Loader;
	bool loadout = Loader.LoadFile(fileName);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3u> indices;

	if (loadout)
	{
		int shift = 0;
		// Go through each loaded mesh and out its contents
		for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
		{
			// Copy one of the loaded meshes to be our current mesh
			objl::Mesh curMesh = Loader.LoadedMeshes[i];

			// Print Mesh Name
			//std::cout << "Mesh " << i << ": " << curMesh.MeshName << "\n";

			// Print Vertices
			//std::cout << "Vertices:\n";

			// Go through each vertex and print its number,
			//  position, normal, and texture coordinate
			for (int j = 0; j < curMesh.Vertices.size(); j++)
			{
				vertices.push_back(glm::vec3(
					curMesh.Vertices[j].Position.X,
					curMesh.Vertices[j].Position.Y,
					curMesh.Vertices[j].Position.Y));

				normals.push_back(glm::vec3(
					curMesh.Vertices[j].Normal.X,
					curMesh.Vertices[j].Normal.Y,
					curMesh.Vertices[j].Normal.Z));

				/*std::cout << "V" << j << ": " <<
					"P(" << curMesh.Vertices[j].Position.X << ", " << curMesh.Vertices[j].Position.Y << ", " << curMesh.Vertices[j].Position.Z << ") " <<
					"N(" << curMesh.Vertices[j].Normal.X << ", " << curMesh.Vertices[j].Normal.Y << ", " << curMesh.Vertices[j].Normal.Z << ") " <<
					"TC(" << curMesh.Vertices[j].TextureCoordinate.X << ", " << curMesh.Vertices[j].TextureCoordinate.Y << ")\n";*/
			}

			// Print Indices
			//std::cout << "Indices:\n";

			// Go through every 3rd index and print the
			//	triangle that these indices represent
			for (int j = 0; j < curMesh.Indices.size(); j += 3)
			{
				indices.push_back(glm::vec3u(
					curMesh.Indices[j] + shift,
					curMesh.Indices[j + 1] + shift,
					curMesh.Indices[j + 2] + shift));

				/*std::cout << "T" << j / 3 << ": " << curMesh.Indices[j] << ", " << curMesh.Indices[j + 1] << ", " << curMesh.Indices[j + 2] << "\n";*/
			}

			// Print Material
			//std::cout << "Material: " << curMesh.MeshMaterial.name << "\n";
			//std::cout << "Ambient Color: " << curMesh.MeshMaterial.Ka.X << ", " << curMesh.MeshMaterial.Ka.Y << ", " << curMesh.MeshMaterial.Ka.Z << "\n";
			//std::cout << "Diffuse Color: " << curMesh.MeshMaterial.Kd.X << ", " << curMesh.MeshMaterial.Kd.Y << ", " << curMesh.MeshMaterial.Kd.Z << "\n";
			//std::cout << "Specular Color: " << curMesh.MeshMaterial.Ks.X << ", " << curMesh.MeshMaterial.Ks.Y << ", " << curMesh.MeshMaterial.Ks.Z << "\n";
			//std::cout << "Specular Exponent: " << curMesh.MeshMaterial.Ns << "\n";
			//std::cout << "Optical Density: " << curMesh.MeshMaterial.Ni << "\n";
			//std::cout << "Dissolve: " << curMesh.MeshMaterial.d << "\n";
			//std::cout << "Illumination: " << curMesh.MeshMaterial.illum << "\n";
			//std::cout << "Ambient Texture Map: " << curMesh.MeshMaterial.map_Ka << "\n";
			//std::cout << "Diffuse Texture Map: " << curMesh.MeshMaterial.map_Kd << "\n";
			//std::cout << "Specular Texture Map: " << curMesh.MeshMaterial.map_Ks << "\n";
			//std::cout << "Alpha Texture Map: " << curMesh.MeshMaterial.map_d << "\n";
			//std::cout << "Bump Map: " << curMesh.MeshMaterial.map_bump << "\n";

			//// Leave a space to separate from the next mesh
			//std::cout << "\n";

			shift += curMesh.Vertices.size();
		}

		for (int i = 0; i < vertices.size(); i++)
		{
			colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
		}
	}
	else
	{
		std::cout << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";
	}

	return new Model(vertices, colors, indices, normals);
}
