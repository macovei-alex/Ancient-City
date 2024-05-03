#pragma comment (lib, "glfw3.lib")
#pragma comment (lib, "glew32s.lib")
#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "assimp-vc143-mt.lib")

#include "utils.h"
#include "KeyBinder.tpp"
#include "Camera.h"
#include "ShaderProgram.h"
#include "LightSource.h"
#include "ModelLoader.h"

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <glew.h>

#include <glfw3.h>

#include <gtc/matrix_transform.hpp>

constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 600;

float deltaTime = 0.0f;
double lastFrame = 0.0f;

ShaderProgram* modelShaders, * lightingShaders, * textureShaders;
Camera* camera;
LightSource* sun;

std::vector<std::unique_ptr<Model>> models;

static void DisplayFPS(double currentTime)
{
	static int frameCounter = 0;
	static int lastPrint = (int)glfwGetTime();

	frameCounter++;

	if (currentTime - lastPrint >= 1)
	{
		std::cout << "FPS: " << frameCounter << std::endl;
		frameCounter = 0;
		lastPrint = (int)currentTime;
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
	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->MoveUp(time);
	if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
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
		sun->AddSpecularStrength(0.1f);
	else if (key == GLFW_KEY_V && action == GLFW_PRESS)
		sun->AddSpecularStrength(-0.1f);
	else if (key == GLFW_KEY_B && action == GLFW_PRESS)
		sun->AddDiffuseStrength(0.1f);
	else if (key == GLFW_KEY_N && action == GLFW_PRESS)
		sun->AddDiffuseStrength(-0.1f);
	else if (key == GLFW_KEY_M && action == GLFW_PRESS)
		sun->MultiplySpecularExponent(2);
	else if (key == GLFW_KEY_COMMA && action == GLFW_PRESS)
		sun->MultiplySpecularExponent(1.0f / 2);

	if(action == GLFW_PRESS)
		std::cout << "Key pressed: " << GetKeyPressed(key) << std::endl;
}

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	camera->Set(width, height);
}

static void MouseCallback(GLFWwindow* window, double deltaX, double deltaY)
{
	camera->HandlMouseMovement(static_cast<float>(deltaX), static_cast<float>(deltaY));
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
}

static void RenderFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	textureShaders->Use();

	textureShaders->SetVec3("LightColor", sun->GetColor());
	textureShaders->SetVec3("LightPosition", sun->GetPosition());
	textureShaders->SetVec3("ViewPosition", camera->GetPosition());

	textureShaders->SetFloat("AmbientStrength", sun->GetAmbientStrength());
	textureShaders->SetFloat("DiffuseStrength", sun->GetDiffuseStrength());
	textureShaders->SetFloat("SpecularStrength", sun->GetSpecularStrength());
	textureShaders->SetInt("SpecularExponent", sun->GetSpecularExponent());

	textureShaders->SetMat4("ViewMatrix", camera->GetViewMatrix());
	textureShaders->SetMat4("ProjectionMatrix", camera->GetProjectionMatrix());

	for (const auto& model : models)
	{
		textureShaders->SetMat4("ModelMatrix", model->GetModelMatrix());
		model->Render(*textureShaders);
	}

	// light source

	modelShaders->Use();

	modelShaders->SetMat4("ModelMatrix", sun->model.GetModelMatrix());
	modelShaders->SetMat4("ViewMatrix", camera->GetViewMatrix());
	modelShaders->SetMat4("ProjectionMatrix", camera->GetProjectionMatrix());

	sun->model.Render(*modelShaders);
}

static void LoadModels()
{
	glm::mat4 onLoadTransforms = glm::mat4(1.0f);
	onLoadTransforms = glm::translate(onLoadTransforms, glm::vec3(0.0f, -2.0f, 0.0f));
	onLoadTransforms = glm::scale(onLoadTransforms, glm::vec3(0.05f, 0.05f, 0.05f));
	onLoadTransforms = glm::rotate(onLoadTransforms, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	onLoadTransforms = glm::rotate(onLoadTransforms, glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	models.emplace_back(ModelLoader::LoadModel("Models\\Wolf\\Wolf.obj", onLoadTransforms));
	models.emplace_back(ModelLoader::LoadModel("Models\\Pirat\\pirat.obj", glm::translate(glm::mat4(1), glm::vec3(2, 0, 0))));
}

int main()
{
	GLFWwindow* window = InitializeWindow();
	if (window == nullptr)
	{
		return -1;
	}
	InitializeGraphics();

	modelShaders = new ShaderProgram("Shaders\\modelVS.glsl", "Shaders\\modelFS.glsl");
	lightingShaders = new ShaderProgram("Shaders\\lightingVS.glsl", "Shaders\\lightingFS.glsl");
	textureShaders = new ShaderProgram("Shaders\\textureVS.glsl", "Shaders\\textureFS.glsl");

	camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);

	LoadModels();

	sun = new LightSource(std::move(*ModelLoader::LoadModel("Models\\Sphere\\sphere.obj", 0.002f)));
	sun->model.SetPosition(camera->GetPosition() + glm::vec3(0.0f, 2.0f, -2.0f));

	while (!glfwWindowShouldClose(window))
	{
		double currentFrame = glfwGetTime();
		deltaTime = (float)(currentFrame - lastFrame);
		lastFrame = currentFrame;

		models[0]->Rotate(glm::vec3(0.0f, deltaTime, 0.0f));
		//lightSource->model.Rotate(glm::vec3(0.0f, deltaTime, 0.0f));

		DisplayFPS(currentFrame);
		PerformKeysActions(window);
		RenderFrame();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Clean();
	return 0;
}