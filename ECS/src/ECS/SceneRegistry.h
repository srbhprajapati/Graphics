#pragma once


#include "Resources/ResourceManager.h"
#include "entt.hpp"
#include "Entity.h"
#include "Common/SceneData.h"

class SceneRegistry {

public:
	SceneRegistry(ResourceManager& manager, SceneData& sData);


	void Initialize(SceneData& sData);

	Entity CreateEntity();
	Entity GetEntity(entt::entity e);
	void DeleteEntity(Entity e);

	entt::registry Registry;

private:
	
	void AddMaterials(std::vector<MaterialData>& materials);
	Entity ParseSceneNode(std::unique_ptr<SceneNode>& node, Entity& parent);

	void AddMeshComponent(Entity& node, const MeshNode* sNode);
	void AddTransformComponent(Entity& node, const SceneNode* sNode);
	void AddCameraComponent(Entity& node, const CameraNode* sNode);
	void AddLightComponent(Entity& node, const LightNode* sNode);

	ResourceManager& resourceManager;
};