#pragma once

#include "System.h"
#include "entt.hpp"
#include "Resources/ResourceManager.h"


class RenderSystem : public System
{
public:

	RenderSystem(entt::registry& registry, ResourceManager& resManager);
	~RenderSystem();

	void BeginFrame() override;
	void Update(float deltaTime) override;
	void EndFrame() override;

private :
	entt::registry& reg;
	ResourceManager& resManager;

	float angle = 0.0f;
};