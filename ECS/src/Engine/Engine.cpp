#include "Engine.h"
#include "AssetLoader/FBXLoader.h"

Engine::Engine()
{
	std::string fbxDatasetPath = "C:/Users/srbhp/Documents/Data/Bistro_v5_2/BistroInterior_Wine.fbx";

	SceneData sceneData;
	FBXLoader::LoadFBXFile(fbxDatasetPath, &sceneData);

	resourceManager = std::make_unique<ResourceManager>();
	scene = std::make_unique<Scene>(*resourceManager, sceneData);

}

Engine::~Engine()
{
}


