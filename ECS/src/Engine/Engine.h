#pragma once

#include "Resources/ResourceManager.h"
#include "ECS/SceneRegistry.h"
#include "Window/Window.h"
#include "Systems/SystemManager.h"

class Engine
{
public:

	Engine();
	~Engine();

	void Run();

private:

	std::unique_ptr<ResourceManager> resourceManager;
	std::unique_ptr<SceneRegistry> scene;
	std::unique_ptr<Window> window;
	std::unique_ptr<SystemManager> systemManager;
};