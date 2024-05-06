#include "Skybox.h"

#include <stb_image.h>

const std::array<float, 24> Skybox::vertices =
{
	-1.0f, -1.0f,  1.0f, //        7--------6
	 1.0f, -1.0f,  1.0f, //       /|       /|
	 1.0f, -1.0f, -1.0f, //      4--------5 |
	-1.0f, -1.0f, -1.0f, //      | |      | |
	-1.0f,  1.0f,  1.0f, //      | 3------|-2
	 1.0f,  1.0f,  1.0f, //      |/       |/
	 1.0f,  1.0f, -1.0f, //      0--------1
	-1.0f,  1.0f, -1.0f
};

const std::array<unsigned int, 36> Skybox::indices =
{
	1, 2, 6, // Right
	6, 5, 1,
	0, 4, 7, // Left
	7, 3, 0,
	4, 5, 6, // Top
	6, 7, 4,
	0, 3, 2, // Bottom
	2, 1, 0,
	0, 1, 5, // Back
	5, 4, 0,
	3, 7, 6, // Front
	6, 2, 3
};

const std::array<std::string, 6> Skybox::facesCubemap =
{
	"\\right.jpg", "\\left.jpg",
	"\\top.jpg", "\\bottom.jpg",
	"\\front.jpg", "\\back.jpg"
};

Skybox::Skybox(const std::string& dirPath)
{
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &VBO));
	GLCall(glGenBuffers(1, &EBO));

	GLCall(glBindVertexArray(VAO));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Skybox::vertices), Skybox::vertices.data(), GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(Skybox::vertices[0]), (void*)0));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Skybox::indices), Skybox::indices.data(), GL_STATIC_DRAW));

	GLCall(glBindVertexArray(0));

	GLCall(glGenTextures(1, &textureID));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, textureID));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

	// This might help with seams on some systems
	GLCall(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));

	for (int i = 0; i < Skybox::facesCubemap.size(); i++)
	{
		int width, height, nrChannels;

		if(i == 0)
			stbi_set_flip_vertically_on_load(true);
		else
			stbi_set_flip_vertically_on_load(false);

		unsigned char* data = stbi_load((dirPath + Skybox::facesCubemap[i]).c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
			stbi_image_free(data);
		}
		else
		{
			LOG(std::format("Failed to load texture: {}\n", dirPath + facesCubemap[i]), Logger::Level::Error);
			stbi_image_free(data);
			return;
		}
	}

	LOG("Successfuly created the sky box", Logger::Level::Info);
}

void Skybox::Render(Shader& skyboxShader, const Camera& camera) const
{
	skyboxShader.Use();
	GLCall(glDepthFunc(GL_LEQUAL));
	GLCall(glCullFace(GL_FRONT));

	glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	glm::mat4 projection = camera.GetProjectionMatrix();

	skyboxShader.SetMat4("ViewMatrix", view);
	skyboxShader.SetMat4("ProjectionMatrix", projection);
	skyboxShader.SetInt("SkyboxTexture", 0);

	GLCall(glBindVertexArray(VAO));

	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, textureID));
	GLCall(glDrawElements(GL_TRIANGLES, (GLsizei)Skybox::indices.size(), GL_UNSIGNED_INT, 0));

	GLCall(glBindVertexArray(0));

	GLCall(glDepthFunc(GL_LESS));
	GLCall(glCullFace(GL_BACK));
}
