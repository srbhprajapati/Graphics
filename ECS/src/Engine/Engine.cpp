#include "Engine.h"
#include "AssetLoader/FBXLoader.h"
#include "Window/GLFWWindow.h"

Engine::Engine()
{
	std::string fbxDatasetPath = "C:/Users/srbhp/Documents/Data/Bistro_v5_2/BistroInterior_Wine.fbx";

	SceneData sceneData;
	FBXLoader::LoadFBXFile(fbxDatasetPath, &sceneData);

	window = std::make_unique<GLFWWindow>(800, 800, "ECS Window");
	resourceManager = std::make_unique<ResourceManager>();
	scene = std::make_unique<SceneRegistry>(*resourceManager, sceneData);

}

Engine::~Engine()
{
}


void Engine::Run()
{
	while (!window->ShouldClose())
	{
		window->PollEvents();



		window->SwapBuffers();
	}
}