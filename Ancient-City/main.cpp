#pragma comment (lib, "glfw3.lib")
#pragma comment (lib, "glew32s.lib")
#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "assimp-vc143-mt.lib")

#include "utils.h"
#include "KeyBinder.tpp"
#include "Camera.h"
#include "Shader.h"
#include "LightSource.h"
#include "Sun.h"
#include "ModelLoader.h"
#include "hotReload.hpp"
#include "Skybox.h"
#include "ParticleGenerator.h"
#include "names.h"

struct Options
{
	bool hotReloadShaders = false;
};

Options options;

constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 600;

float deltaTime = 0.0f;
double lastFrame = 0.0f;

Shader* modelShaders, * textureShaders = nullptr, * skyboxShaders = nullptr, * particleShaders = nullptr, * shadowShaders = nullptr, * depthMapShaders = nullptr;
Camera* camera = nullptr;
Sun* sun = nullptr;
Skybox* skybox = nullptr;

std::vector<Model*> models;
std::vector<ParticleGenerator*> particleGenerators;

static void DisplayFPS(double currentTime)
{
	static uint totalFrameCounter = 0;
	static uint frameCounter = 0;
	static double lastPrint = glfwGetTime();
	static uint printCount = 0;

	frameCounter++;

	if (currentTime - lastPrint >= 1)
	{
		printCount++;
		totalFrameCounter += frameCounter;
		std::cout << "FPS: " << frameCounter << "  AVG: " << totalFrameCounter / printCount << std::endl;
		frameCounter = 0;
		lastPrint = currentTime;
	}
}

static void PerformKeysActions(GLFWwindow* window)
{
	float time = deltaTime;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		time *= Camera::SPEED_BOOST_MULTIPLIER;
	if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		time *= Camera::SPEED_SLOW_MULTIPLIER;

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

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		sun->Rotate(10 * time, 0, 0);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		sun->Rotate(-10 * time, 0, 0);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		sun->Rotate(0, 0, 10 * time);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		sun->Rotate(0, 0, -10 * time);
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		camera->Set(width, height, Camera::START_POSITION);
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
		sun->MultiplySpecularExponent(2.0f);
	else if (key == GLFW_KEY_COMMA && action == GLFW_PRESS)
		sun->MultiplySpecularExponent(0.5f);

	if (action == GLFW_PRESS)
		std::cout << "Key pressed: " << GetKeyPressed(key) << std::endl;
}

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	camera->Set(width, height, camera->GetPosition());
}

static void MouseCallback(GLFWwindow* window, double deltaX, double deltaY)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->HandleMouseMovement((float)deltaX, (float)deltaY);
	else
		camera->SetLastMousePos((float)deltaX, (float)deltaY);
}

static void ScrollCallback(GLFWwindow* window, double xoffset, double yOffset)
{
	camera->HandleMouseScroll((float)yOffset);
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
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ancient City", NULL, NULL);
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
	delete modelShaders, textureShaders, particleShaders, skyboxShaders, shadowShaders, depthMapShaders;
	delete camera;
	delete sun;

	for (auto& model : models)
		delete model;

	for (auto& particleGenerator : particleGenerators)
		delete particleGenerator;

	if (options.hotReloadShaders)
		CleanHotReloadHandles();
	glfwTerminate();
}

static void RenderFrame()
{
	// sun->CreateShadowMap(*depthMapShaders, models);

	camera->SetViewPort();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox->Render(*skyboxShaders, *camera);

	shadowShaders->Use();
	shadowShaders->SetInt("ShadowMap", 15);
	shadowShaders->SetUniforms(camera, sun, nullptr, Shader::Uniforms::DefaultOptions);

	for (const auto& model : models)
	{
		shadowShaders->SetUniforms(nullptr, nullptr, model, Shader::Uniforms::ModelMatrix);
		model->Render(*shadowShaders);
	}

	modelShaders->Use();
	modelShaders->SetUniforms(camera, nullptr, &sun->GetModel(),
		Shader::Uniforms::ViewMatrix
		| Shader::Uniforms::ProjectionMatrix
		| Shader::Uniforms::ModelMatrix);
	sun->Render(*modelShaders);

	particleShaders->Use();
	particleShaders->SetUniforms(camera, nullptr, nullptr,
		Shader::Uniforms::ProjectionMatrix
		| Shader::Uniforms::ViewMatrix);
	particleShaders->SetFloat("AmbientStrength", 
		ParticleGenerator::CalculateAmbientStrength(sun->GetAmbientStrength()));
	for (const auto& particleGenerator : particleGenerators)
	{
		particleGenerator->RenderParticles(*particleShaders);
	}
}

static void LoadShader(const std::string& shaderFilesIdentifier, bool mustCompile)
{
	Shader** targetedShaderPtr = nullptr;

	if (shaderFilesIdentifier == names::shaders::model)
		targetedShaderPtr = &modelShaders;
	else if (shaderFilesIdentifier == names::shaders::texture)
		targetedShaderPtr = &textureShaders;
	else if (shaderFilesIdentifier == names::shaders::skybox)
		targetedShaderPtr = &skyboxShaders;
	else if (shaderFilesIdentifier == names::shaders::particle)
		targetedShaderPtr = &particleShaders;
	else if (shaderFilesIdentifier == names::shaders::shadow)
		targetedShaderPtr = &shadowShaders;
	else if (shaderFilesIdentifier == names::shaders::depthMap)
		targetedShaderPtr = &depthMapShaders;

	else
	{
		LOG(std::format("Unknown shader identifier: {}", shaderFilesIdentifier), Logger::Level::Warning);
		return;
	}

	Shader* newShader = new Shader(
		std::format("{}\\{}{}", names::shaders::dirName, shaderFilesIdentifier, names::shaders::vertex),
		std::format("{}\\{}{}", names::shaders::dirName, shaderFilesIdentifier, names::shaders::fragment));
	if (newShader->GetID() != -1)
	{
		if (*targetedShaderPtr != nullptr)
			delete* targetedShaderPtr;
		*targetedShaderPtr = newShader;
	}
	else
	{
		std::string errorString = std::format("Shader ( {} ) could not compile", shaderFilesIdentifier);
		if (mustCompile)
		{
			LOG(errorString, Logger::Level::Error);
			throw std::runtime_error(errorString);
		}
		else
		{
			LOG(errorString, Logger::Level::Warning);
		}
	}
}

static void SetupWorld()
{
	camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT, Camera::START_POSITION);
	skybox = new Skybox("Models\\Skybox");

	models.push_back(ModelLoader::LoadModel("Models\\Castle\\Castle OBJ.obj",
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f))));

	Model* sphere = ModelLoader::LoadModel("Models\\Sphere\\sphere.obj", 0.002f);
	sun = new Sun(*sphere);
	sun->GetModel().Scale(100.0f);

	auto gen = &(new ParticleGenerator(*sphere))
		->WithSpeedModifier(2.0f)
		.WithLifeTime(2.0f)
		.WithPosition(-2.0f, 0.0f, 0.0f)
		.WithParticleColor(1.0f, 0.5f, 0.0f)
		.WithParticleAlphaFade(true);
	particleGenerators.push_back(gen);

	gen = &(new ParticleGenerator(*sphere))
		->WithSpeedModifier(2.0f)
		.WithLifeTime(2.0f)
		.WithStartingParticleColor(1.0f, 0.2f, 0.0f)
		.WithEndingParticleColor(0.0f, 0.5f, 0.7f)
		.WithScale(2.0f);
	particleGenerators.push_back(gen);

	gen = &(new ParticleGenerator())
		->WithSpeedModifier(2.0f)
		.WithLifeTime(2.0f)
		.WithStartingParticleColor(0.0f, 0.5f, 0.7f)
		.WithEndingParticleColor(1.0f, 0.2f, 0.0f)
		.WithPosition(2.0f, 0.0f, 0.0f)
		.WithParticleAlphaFade(true)
		.WithScale(0.2f);
	particleGenerators.push_back(gen);
}

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--hotRealoadShaders") == 0)
			{
				options.hotReloadShaders = true;
				AddHotReloadDir(names::shaders::dirName + '\\');
			}
		}
	}

	GLFWwindow* window = InitializeWindow();
	if (window == nullptr)
	{
		return -1;
	}
	InitializeGraphics();

	LoadShader(names::shaders::model, true);
	LoadShader(names::shaders::texture, true);
	LoadShader(names::shaders::skybox, true);
	LoadShader(names::shaders::particle, true);
	LoadShader(names::shaders::depthMap, true);
	LoadShader(names::shaders::shadow, true);

	SetupWorld();

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
					? TrimBeginEnd(file, names::shaders::dirName + '\\', names::shaders::vertex)
					: TrimBeginEnd(file, names::shaders::dirName + '\\', names::shaders::fragment);
				LoadShader(shaderIdentifier, false);
			}
		}

		for (auto& particleGenerators : particleGenerators)
		{
			particleGenerators->MoveParticles(deltaTime);
			particleGenerators->SpawnParticles(deltaTime);
		}

		sun->PassTime((float)deltaTime);

		PerformKeysActions(window);
		RenderFrame();
		DisplayFPS(currentFrame);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Clean();
	return 0;
}