#pragma comment (lib, "glfw3.lib")
#pragma comment (lib, "glew32s.lib")
#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "assimp-vc143-mt.lib")

#include "utils.h"
#include "FlyingCamera.h"
#include "WalkingCamera.h"
#include "Shader.h"
#include "Sun.h"
#include "ModelLoader.h"
#include "hotReload.hpp"
#include "Skybox.h"
#include "ParticleGenerator.h"
#include "constants.h"
#include "Batch.h"

struct Options
{
	bool hotReloadShaders = false;
	bool batchRendering = true;
	bool walkingCamera = false;
};

struct WorldState
{
	const unsigned int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;
	const uint SHADOW_RES_WIDTH = 1024, SHADOW_RES_HEIGHT = 1024;

	float deltaTime, lastFrame;
	bool sunStop = false;

	inline void Init()
	{
		deltaTime = (float)glfwGetTime();
		lastFrame = (float)glfwGetTime();
	}
};

Options options;
WorldState worldState;

Shader* modelShaders, * textureShaders = nullptr, * skyboxShaders = nullptr, * particleShaders = nullptr, * shadowShaders = nullptr, * depthMapShaders = nullptr, * cubeShaders = nullptr, * quadTextureShaders = nullptr;
BaseCamera* camera = nullptr;
Sun* sun = nullptr;
Skybox* daySkybox = nullptr, * nightSkybox = nullptr;

std::vector<Model*> models;
std::vector<Batch*> batches;
std::vector<ParticleGenerator*> particleGenerators;

static void SetupOptions(int argc, char* argv[])
{
	if (argc == 1)
		return;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--hotRealoadShaders") == 0)
		{
			options.hotReloadShaders = true;
			AddHotReloadDir(names::shaders::dirName + '\\');
		}

		else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--modelRendering") == 0)
		{
			options.batchRendering = false;
		}

		else if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--batchRendering") == 0)
		{
			options.batchRendering = true;
		}

		else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--flyingCamera") == 0)
		{
			options.walkingCamera = false;
		}

		else if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--walkingCamera") == 0)
		{
			options.walkingCamera = true;
		}
	}
}

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
	float time = worldState.deltaTime;

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		time *= camera->speedBoostMultiplier;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		time *= camera->speedSlowMultiplier;

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
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera->MoveDown(time);

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		sun->PassTime(time);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		sun->PassTime(-time);
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		return;
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		camera->Set(width, height, BaseCamera::START_POSITION);
	}

	else if (key == GLFW_KEY_Z && action == GLFW_PRESS)
		sun->AddAmbientIntensity(0.1f);
	else if (key == GLFW_KEY_X && action == GLFW_PRESS)
		sun->AddAmbientIntensity(-0.1f);
	else if (key == GLFW_KEY_C && action == GLFW_PRESS)
		sun->AddDiffuseIntensity(0.1f);
	else if (key == GLFW_KEY_V && action == GLFW_PRESS)
		sun->AddDiffuseIntensity(-0.1f);
	else if (key == GLFW_KEY_B && action == GLFW_PRESS)
		sun->AddSpecularIntensity(0.1f);
	else if (key == GLFW_KEY_N && action == GLFW_PRESS)
		sun->AddSpecularIntensity(-0.1f);

	else if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
		worldState.sunStop = !worldState.sunStop;

	if (action == GLFW_PRESS)
		std::cout << "Key pressed: " << GetKeyPressed(key) << std::endl;
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		glm::vec3 position = camera->GetPosition();
		glm::vec3 forward = camera->GetForward();
		float param = -position.y / forward.y;
		if (param > 0)
		{
			glm::vec3 intersect = position + param * forward;
			auto fires = ParticleGenerator::NewFire(intersect);
			particleGenerators.insert(particleGenerators.end(), fires.begin(), fires.end());
		}
	}
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

static void InitGraphics()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
}

static GLFWwindow* InitWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(worldState.SCREEN_WIDTH, worldState.SCREEN_HEIGHT, "Ancient City", NULL, NULL);
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
	glfwSetMouseButtonCallback(window, MouseButtonCallback);

	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	glewInit();
	return window;
}

static void Clean()
{
	delete modelShaders, textureShaders, particleShaders, skyboxShaders, shadowShaders, depthMapShaders, cubeShaders;
	delete camera;
	delete sun;

	for (auto& model : models)
		delete model;

	for (auto& particleGenerator : particleGenerators)
		delete particleGenerator;

	for (auto& batch : batches)
		delete batch;

	if (options.hotReloadShaders)
		CleanHotReloadHandles();
	glfwTerminate();
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
	else if (shaderFilesIdentifier == names::shaders::cube)
		targetedShaderPtr = &cubeShaders;
	else if (shaderFilesIdentifier == names::shaders::quadTexture)
		targetedShaderPtr = &quadTextureShaders;

	else
	{
		std::string errorString = std::format("Unknown shader identifier: {}", shaderFilesIdentifier);
		if (mustCompile)
		{
			LOG(errorString, Logger::Level::Error);
			throw std::runtime_error(errorString);
		}
		else
		{
			LOG(errorString, Logger::Level::Warning);
			return;
		}
	}

	Shader* newShader = new Shader(
		std::format("{}\\{}{}", names::shaders::dirName, shaderFilesIdentifier, names::shaders::vertex),
		std::format("{}\\{}{}", names::shaders::dirName, shaderFilesIdentifier, names::shaders::fragment));
	if (newShader->GetID() != -1)
	{
		if (*targetedShaderPtr != nullptr)
			delete* targetedShaderPtr;
		*targetedShaderPtr = newShader;
		LOG(std::format("Shader ( {} ) successfuly compiled", shaderFilesIdentifier), Logger::Level::Info);
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
	if(options.walkingCamera)
		camera = new WalkingCamera(worldState.SCREEN_WIDTH, worldState.SCREEN_HEIGHT, glm::vec3(0.0f, 1.8f, 0.0f));
	else
		camera = new FlyingCamera(worldState.SCREEN_WIDTH, worldState.SCREEN_HEIGHT, glm::vec3(0.0f, 1.8f, 0.0f));

	daySkybox = new Skybox("Models\\Day-Skybox");
	nightSkybox = new Skybox("Models\\Night-Skybox");

	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = glm::translate(matrix, glm::vec3(-205.0f, -5.0f, 0.0f));
	matrix = glm::scale(matrix, glm::vec3(10.0f, 10.0f, 10.0f));
	models.push_back(ModelLoader::LoadModel("Models\\Chinese-Town\\chinese town.obj", matrix));

	Model* sphere = ModelLoader::LoadModel("Models\\Sphere\\sphere.obj", 0.4f);
	sun = new Sun(std::move(*sphere));

	particleGenerators = ParticleGenerator::GetFromFile("Models\\particle generators.txt");

	if (options.batchRendering)
	{
		batches = Batch::SplitIntoBatches(models);
		LOG("Split meshes into batches", Logger::Level::Info);
	}

	delete sphere;
}

static void RenderFrame()
{
	// sun->WriteToShadowMap(*depthMapShaders, models);

	camera->SetViewPort();
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	skyboxShaders->Use();
	daySkybox->Bind(values::textureUnits::daySkybox);
	nightSkybox->Bind(values::textureUnits::nightSkybox);
	skyboxShaders->SetVP(camera->CalculateProjectionMatrix() * camera->CalculateViewMatrix());
	skyboxShaders->SetDaySkyboxTexture(values::textureUnits::daySkybox);
	skyboxShaders->SetNightSkyboxTexture(values::textureUnits::nightSkybox);
	Skybox::RenderCube();

	shadowShaders->Use();
	// sun->GetShadowMap().BindForRead(*shadowShaders);
	shadowShaders->SetVP(camera->CalculateProjectionMatrix() * camera->CalculateViewMatrix());
	shadowShaders->SetLightColor(sun->light.color);
	shadowShaders->SetViewPosition(camera->GetPosition());
	shadowShaders->SetAmbientIntensity(sun->light.ambientIntensity);
	shadowShaders->SetDiffuseIntensity(sun->light.diffuseIntensity);
	shadowShaders->SetSpecularIntensity(sun->light.specularIntensity);

	for (const auto& model : models)
	{
		glm::mat4 modelMatrix = model->modelMatrix;
		glm::mat3 lightTransformation = glm::mat3(modelMatrix);
		lightTransformation = glm::transpose(lightTransformation);

		glm::vec3 lightDirection = lightTransformation * sun->light.direction;
		lightDirection = glm::normalize(lightDirection);

		shadowShaders->SetLightDirection(lightDirection);
		shadowShaders->SetModelMatrix(modelMatrix);

		model->Render(*shadowShaders);
	}

	modelShaders->Use();
	modelShaders->SetMVP(camera->CalculateProjectionMatrix() * camera->CalculateViewMatrix() * sun->model.modelMatrix);
	sun->DepthRender();

	particleShaders->Use();
	particleShaders->SetVP(camera->CalculateProjectionMatrix() * camera->CalculateViewMatrix());
	particleShaders->SetAmbientIntensity(ParticleGenerator::CalculateAmbientIntensity(sun->light.ambientIntensity));
	for (const auto& particleGenerator : particleGenerators)
	{
		particleGenerator->RenderParticles(*particleShaders);
	}
}

static void BatchRenderFrame()
{
	// sun->WriteToShadowMap(*depthMapShaders, models);

	camera->SetViewPort();
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	skyboxShaders->Use();
	daySkybox->Bind(values::textureUnits::daySkybox);
	nightSkybox->Bind(values::textureUnits::nightSkybox);
	glm::mat4 viewMatrix = glm::mat4(glm::mat3(camera->CalculateViewMatrix()));
	skyboxShaders->SetVP(camera->CalculateProjectionMatrix() * viewMatrix);
	skyboxShaders->SetDaySkyboxTexture(values::textureUnits::daySkybox);
	skyboxShaders->SetNightSkyboxTexture(values::textureUnits::nightSkybox);
	skyboxShaders->SetMixPercent(sun->CalculateSkyboxesMixPercent());
	Skybox::RenderCube();

	shadowShaders->Use();
	// sun->GetShadowMap().BindForRead(*shadowShaders);
	shadowShaders->SetVP(camera->CalculateVP());
	shadowShaders->SetLightDirection(sun->light.direction);
	shadowShaders->SetLightColor(sun->light.color);
	shadowShaders->SetViewPosition(camera->GetPosition());
	shadowShaders->SetAmbientIntensity(sun->light.ambientIntensity);
	shadowShaders->SetDiffuseIntensity(sun->light.diffuseIntensity);
	shadowShaders->SetSpecularIntensity(sun->light.specularIntensity);
	shadowShaders->SetModelMatrix(glm::mat4(1.0f));

	// GLCall(glActiveTexture(GL_TEXTURE0 + values::textureUnits::shadowMap));
	// GLCall(glBindTexture(GL_TEXTURE_2D, depthMap));
	// shadowShaders->SetShadowMap(values::textureUnits::shadowMap);

	for (const auto& batch : batches)
	{
		batch->Render(*shadowShaders);
	}

	modelShaders->Use();
	modelShaders->SetMVP(camera->CalculateVP() * sun->model.modelMatrix);
	sun->DepthRender();

	particleShaders->Use();
	particleShaders->SetVP(camera->CalculateVP());
	particleShaders->SetAmbientIntensity(ParticleGenerator::CalculateAmbientIntensity(sun->light.ambientIntensity));
	for (const auto& particleGenerator : particleGenerators)
	{
		particleGenerator->RenderParticles(*particleShaders);
	}
}

int main(int argc, char* argv[])
{
	SetupOptions(argc, argv);

	GLFWwindow* window = InitWindow();
	if (window == nullptr)
	{
		return -1;
	}
	InitGraphics();

	LoadShader(names::shaders::model, true);
	LoadShader(names::shaders::skybox, true);
	LoadShader(names::shaders::particle, true);
	LoadShader(names::shaders::depthMap, true);
	LoadShader(names::shaders::shadow, true);
	LoadShader(names::shaders::cube, true);
	LoadShader(names::shaders::quadTexture, true);

	SetupWorld();
	worldState.Init();

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		worldState.deltaTime = currentFrame - worldState.lastFrame;
		worldState.lastFrame = currentFrame;

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
			particleGenerators->MoveParticles(worldState.deltaTime);
			particleGenerators->SpawnParticles(worldState.deltaTime);
		}

		if (!worldState.sunStop)
		{
			sun->PassTime(worldState.deltaTime);
		}

		PerformKeysActions(window);

		if (options.batchRendering)
			BatchRenderFrame();
		else
			RenderFrame();

		DisplayFPS(currentFrame);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Clean();
	return 0;
}