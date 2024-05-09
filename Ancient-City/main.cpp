#pragma comment (lib, "glfw3.lib")
#pragma comment (lib, "glew32s.lib")
#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "assimp-vc143-mt.lib")

#include "utils.h"
#include "KeyBinder.tpp"
#include "Camera.h"
#include "Shader.h"
#include "LightSource.h"
#include "ModelLoader.h"
#include "hotReload.hpp"
#include "Skybox.h"
#include "ParticleGenerator.h"

#include <gtc/matrix_transform.hpp>
#include <thread>

struct Options
{
	bool hotReloadShaders = false;
};

Options options;

constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 600;

float deltaTime = 0.0f;
double lastFrame = 0.0f;

Shader* modelShaders = nullptr, * lightingShaders = nullptr, * textureShaders = nullptr, * skyboxShaders = nullptr, * particleShader = nullptr;
Camera* camera = nullptr;
LightSource* sun = nullptr;
Skybox* skybox = nullptr;

std::vector<std::unique_ptr<Model>> models;
std::vector<ParticleGenerator> particleGenerators;

static void DisplayFPS(double currentTime)
{
	static int frameCounter = 0;
	static double lastPrint = glfwGetTime();

	frameCounter++;

	if (currentTime - lastPrint >= 1)
	{
		std::cout << "FPS: " << frameCounter << std::endl;
		frameCounter = 0;
		lastPrint = currentTime;
	}
}

static void PerformKeysActions(GLFWwindow* window)
{
	float time = deltaTime;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		time *= Camera::SPEED_BOOST_MULTIPLIER;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->MoveForward(time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->MoveBackward(time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->MoveLeft(time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->MoveRight(time);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->MoveUp(time);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera->MoveDown(time);
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		camera->Set(width, height);
	}

	else if (key == GLFW_KEY_Z && action == GLFW_PRESS)
		sun->AddAmbientStrength(0.1f);
	else if (key == GLFW_KEY_X && action == GLFW_PRESS)
		sun->AddAmbientStrength(-0.1f);
	else if (key == GLFW_KEY_C && action == GLFW_PRESS)
		sun->AddDiffuseStrength(0.1f);
	else if (key == GLFW_KEY_V && action == GLFW_PRESS)
		sun->AddDiffuseStrength(-0.1f);
	else if (key == GLFW_KEY_B && action == GLFW_PRESS)
		sun->AddSpecularStrength(0.1f);
	else if (key == GLFW_KEY_N && action == GLFW_PRESS)
		sun->AddSpecularStrength(-0.1f);
	else if (key == GLFW_KEY_M && action == GLFW_PRESS)
		sun->MultiplySpecularExponent(2);
	else if (key == GLFW_KEY_COMMA && action == GLFW_PRESS)
		sun->MultiplySpecularExponent(1.0f / 2);

	if (action == GLFW_PRESS)
		std::cout << "Key pressed: " << GetKeyPressed(key) << std::endl;
}

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	camera->Set(width, height);
}

static void MouseCallback(GLFWwindow* window, double deltaX, double deltaY)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->HandleMouseMovement(static_cast<float>(deltaX), static_cast<float>(deltaY));
	else
		camera->SetLastMousePos(static_cast<float>(deltaX), static_cast<float>(deltaY));
}

static void ScrollCallback(GLFWwindow* window, double xoffset, double yOffset)
{
	camera->HandleMouseScroll(static_cast<float>(yOffset));
}

static void InitializeGraphics()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
}

static GLFWwindow* InitializeWindow()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Model Viewer", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwSetWindowPos(window, 400, 200);

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	glewInit();
	return window;
}

static void Clean()
{
	delete modelShaders, lightingShaders;
	delete camera;
	delete sun;

	glfwTerminate();

	if(options.hotReloadShaders)
		CleanHotReloadHandles();
}

static void RenderFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox->Render(*skyboxShaders, *camera);

	textureShaders->Use();
	textureShaders->SetUniforms(camera, sun, nullptr, Shader::Uniforms::DefaultOptions);

	for (const auto& model : models)
	{
		textureShaders->SetUniforms(nullptr, nullptr, model.get(), Shader::Uniforms::ModelMatrix);
		model->Render(*textureShaders);
	}

	modelShaders->Use();
	modelShaders->SetUniforms(camera, nullptr, &sun->model, Shader::Uniforms::ViewMatrix 
		| Shader::Uniforms::ProjectionMatrix
		| Shader::Uniforms::ModelMatrix);
	sun->model.Render(*modelShaders);

	particleShader->Use();
	particleShader->SetMat4("ProjectionMatrix", camera->GetProjectionMatrix());
	for (const auto& particleGenerator : particleGenerators)
	{
		particleGenerator.RenderParticles(*particleShader);
	}
}
static void LoadShader(const std::string& shaderFilesIdentifier)
{
	Shader** targetedShaderPtr = nullptr;
	if (shaderFilesIdentifier == "model")
		targetedShaderPtr = &modelShaders;

	else if (shaderFilesIdentifier == "lighting")
		targetedShaderPtr = &lightingShaders;

	else if (shaderFilesIdentifier == "texture")
		targetedShaderPtr = &textureShaders;

	else if (shaderFilesIdentifier == "skybox")
		targetedShaderPtr = &skyboxShaders;

	else if(shaderFilesIdentifier == "particle")
		targetedShaderPtr = &particleShader;

	else
	{
		std::cout << "Unknown shader identifier: " << shaderFilesIdentifier << std::endl;
		return;
	}

	Shader* newShader = new Shader(
		std::format("Shaders\\{}VS.glsl", shaderFilesIdentifier),
		std::format("Shaders\\{}FS.glsl", shaderFilesIdentifier));
	if (newShader->GetID() != -1)
	{
		if (*targetedShaderPtr != nullptr)
			delete* targetedShaderPtr;
		*targetedShaderPtr = newShader;
	}
}

static void SetupWorld()
{
	models.emplace_back(ModelLoader::LoadModel("Models\\Castle\\Castle OBJ.obj",
		glm::translate(glm::mat4(1), glm::vec3(0, -2, 0))));

	particleGenerators.push_back(ParticleGenerator(0.1f));
}

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
			if (std::strcmp(argv[i], "-r") == 0 || std::strcmp(argv[i], "--hotRealoadShaders") == 0)
			{
				options.hotReloadShaders = true;
				AddHotReloadDir("Shaders\\");
			}
	}

	GLFWwindow* window = InitializeWindow();
	if (window == nullptr)
	{
		return -1;
	}
	InitializeGraphics();

	LoadShader("model");
	LoadShader("lighting");
	LoadShader("texture");
	LoadShader("skybox");
	LoadShader("particle");

	camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);

	SetupWorld();

	sun = new LightSource(*ModelLoader::LoadModel("Models\\Sphere\\sphere.obj", 0.002f));
	sun->SetPosition(camera->GetPosition() + glm::vec3(0.0f, 0.0f, -2.0f));

	skybox = new Skybox("Models\\Skybox");

	while (!glfwWindowShouldClose(window))
	{
		double currentFrame = glfwGetTime();
		deltaTime = (float)(currentFrame - lastFrame);
		lastFrame = currentFrame;

		if (options.hotReloadShaders)
		{
			std::vector<std::string> changedFiles = CheckHotReload();
			for (const auto& file : changedFiles)
			{
				std::string shaderIdentifier = file[file.size() - 7] == 'V' 
					? TrimBeginEnd(file, "Shaders\\", "VS.glsl")
					: TrimBeginEnd(file, "Shaders\\", "FS.glsl");
				LoadShader(shaderIdentifier);
			}
		}

		for (auto& particleGenerators : particleGenerators)
		{
			particleGenerators.MoveParticles(deltaTime);
			particleGenerators.SpawnParticles(deltaTime);
		}

		DisplayFPS(currentFrame);
		PerformKeysActions(window);
		RenderFrame();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Clean();
	return 0;
}