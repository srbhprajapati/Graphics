#pragma once

#include "entt.hpp"
#include "Entity.h"
#include "Common/SceneData.h"


class SceneGraph {

public:
	SceneGraph(SceneData& sData);


	void Initialize(SceneData& sData);

	Entity CreateEntity();
	Entity GetEntity(entt::entity e);
	void DeleteEntity(Entity e);



	entt::registry SceneRegistry;

private:
	
	Entity ParseSceneNode(std::unique_ptr<SceneNode>& node, Entity& parent);

	void AddMeshComponent(Entity& node, const MeshNode* sNode);
	void AddCameraComponent(Entity& node, const CameraNode* sNode);
	void AddLightComponent(Entity& node, const LightNode* sNode);
};