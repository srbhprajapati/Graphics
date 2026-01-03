#include "Engine.h"
#include "AssetLoader/FBXLoader.h"
#include "Window/GLFWWindow.h"
#include "Systems/RenderSystem.h"
#include <chrono>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

using stclock = std::chrono::steady_clock;

Engine::Engine()
{
	std::string fbxDatasetPath = "C:/Users/srbhp/Documents/Data/Bistro_v5_2/BistroInterior_Wine.fbx";

	SceneData sceneData;
	FBXLoader::LoadFBXFile(fbxDatasetPath, &sceneData);

	window = std::make_unique<GLFWWindow>(WINDOW_WIDTH, WINDOW_HEIGHT, "ECS Window");
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
	auto lastTime = stclock::now();
	while (!window->ShouldClose())
	{
		auto now = stclock::now();
		float deltaTime = std::chrono::duration<float, std::milli>(now - lastTime).count();
		lastTime = now;

		window->PollEvents();



		//Update Input System Here

		//Update Physics System here




		//Rendering
		systemManager->GetSystem<RenderSystem>().BeginFrame();
		systemManager->GetSystem<RenderSystem>().Update(deltaTime);
		systemManager->GetSystem<RenderSystem>().EndFrame();



		window->SwapBuffers();
	}
}