#include <iostream>

#include "AssetLoader/FBXLoader.h"
#include "Common/SceneData.h"
#include "ECS/SceneRegistry.h"
#include "Engine/Engine.h"


int main(int argc, char** argv)
{
	std::unique_ptr<Engine> engine = std::make_unique<Engine>();

	engine->Run();

	return 0;
}