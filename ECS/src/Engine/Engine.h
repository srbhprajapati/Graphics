#pragma once

#include "Resources/ResourceManager.h"
#include "Core/Scene.h"

class Engine
{
public:

	Engine();
	~Engine();

private:

	std::unique_ptr<ResourceManager> resourceManager;
	std::unique_ptr<Scene> scene;
};