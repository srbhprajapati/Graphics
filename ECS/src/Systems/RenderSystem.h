#pragma once

#include "System.h"
#include "entt.hpp"
#include "Resources/ResourceManager.h"


class RenderSystem : public System
{
public:

	RenderSystem(entt::registry& registry, ResourceManager& resManager);
	~RenderSystem();


	void Update(float deltaTime) override;

private :
	entt::registry& reg;
	ResourceManager& resManager;
};