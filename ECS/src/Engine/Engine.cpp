#include "Engine.h"
#include "AssetLoader/FBXLoader.h"
#include "Window/GLFWWindow.h"
#include "Systems/RenderSystem.h"
#include <chrono>

using clock = std::chrono::steady_clock;

Engine::Engine()
{
	std::string fbxDatasetPath = "C:/Users/srbhp/Documents/Data/Bistro_v5_2/BistroInterior_Wine.fbx";

	SceneData sceneData;
	FBXLoader::LoadFBXFile(fbxDatasetPath, &sceneData);

	window = std::make_unique<GLFWWindow>(800, 800, "ECS Window");
	resourceManager = std::make_unique<ResourceManager>();
	scene = std::make_unique<SceneRegistry>(*resourceManager, sceneData);
	systemManager = std::make_unique<SystemManager>();

	systemManager->AddSystem<RenderSystem>(scene->Registry, *resourceManager);

}

Engine::~Engine()
{
}


void Engine::Run()
{
	auto lastTime = clock::now();
	while (!window->ShouldClose())
	{
		auto now = clock::now();
		float deltaTime = std::chrono::duration<float, std::milli>(now - lastTime).count();
		lastTime = now;

		window->PollEvents();

		//scene->Update();
		systemManager->UpdateAll(deltaTime);


		window->SwapBuffers();
	}
}