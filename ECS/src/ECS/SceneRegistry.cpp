#include "SceneRegistry.h"
#include "Entity.h"
#include "Components.h"

SceneRegistry::SceneRegistry(ResourceManager& manager, SceneData& sData) : resourceManager(manager)
{
	Initialize(sData);
}

void SceneRegistry::Initialize(SceneData& sData)
{
	AddMaterials(sData.Materials);
	ParseSceneNode(sData.rootNode, Entity(entt::null, this));
}


void SceneRegistry::AddMaterials(std::vector<MaterialData>& materials)
{
	for (auto& material : materials)
	{
		resourceManager.CreateMaterial(material);
	}
}


Entity SceneRegistry::CreateEntity()
{
	auto eid = Registry.create();
	return Entity(eid, this);
}


Entity SceneRegistry::GetEntity(entt::entity e)
{
	return Entity(e, this);
}

void SceneRegistry::DeleteEntity(Entity e)
{
	Registry.destroy(e.GetID());
}

Entity SceneRegistry::ParseSceneNode(std::unique_ptr<SceneNode>& node, Entity& parent)
{
	Entity nodeEntity = CreateEntity();
	
	//Relationship Component
	auto& currNodeRelComp = nodeEntity.AddComponent<RelationshipComponent>();
	currNodeRelComp.parent = parent.GetID();
	
	if (parent.GetID() != entt::null)
	{
		auto& parentRelComp = parent.GetComponent<RelationshipComponent>();

		if (parentRelComp.firstChild == entt::null)
		{
			parentRelComp.firstChild = nodeEntity.GetID();
		}
		else
		{
			auto lastSibling = parentRelComp.firstChild;
			while (GetEntity(lastSibling).GetComponent<RelationshipComponent>().nextSibling != entt::null)
			{
				lastSibling = GetEntity(lastSibling).GetComponent<RelationshipComponent>().nextSibling;
			}

			GetEntity(lastSibling).GetComponent<RelationshipComponent>().nextSibling = nodeEntity.GetID();
			currNodeRelComp.prevSibling = lastSibling;
		}
	}

	for (auto& child : node->children)
	{
		ParseSceneNode(child, nodeEntity);
	}


	//Mesh Component
	if (auto meshNode = dynamic_cast<const MeshNode*>(node.get()))
	{
		AddMeshComponent(nodeEntity, meshNode);
		AddTransformComponent(nodeEntity, meshNode);
	}

	//Camera Component
	if (auto cameraNode = dynamic_cast<const CameraNode*>(node.get()))
	{
		AddCameraComponent(nodeEntity, cameraNode);
		AddTransformComponent(nodeEntity, cameraNode);
	}

	//Light Component
	if (auto lightNode = dynamic_cast<const LightNode*>(node.get()))
	{
		AddLightComponent(nodeEntity, lightNode);
		AddTransformComponent(nodeEntity, lightNode);
	}

	return nodeEntity;
}

void SceneRegistry::AddMeshComponent(Entity& node, const MeshNode* sNode)
{
	MeshHandle mesh = resourceManager.CreateMesh(sNode->vertices, sNode->normals, sNode->uvs, sNode->indices, sNode->materialIndex);

	auto& meshComponent = node.AddComponent<MeshComponent>();
	meshComponent.mesh = mesh;
}

void SceneRegistry::AddCameraComponent(Entity& node, const CameraNode* sNode)
{
	auto& cameraComponent = node.AddComponent<CameraComponent>();
	cameraComponent.cameraIndex = sNode->cameraIndex;
	cameraComponent.projectionMatrix = sNode->projectionMatrix;
}

void SceneRegistry::AddLightComponent(Entity& node, const LightNode* sNode)
{
	auto& lightComponent = node.AddComponent<LightComponent>();
	lightComponent.lightIndex = sNode->lightIndex;
}


void SceneRegistry::AddTransformComponent(Entity& node, const SceneNode* sNode)
{
	auto& transformComponent = node.AddComponent<TransformComponent>();

	transformComponent.position = sNode->transform.position;
	transformComponent.rotation = sNode->transform.rotation;
	transformComponent.scale = sNode->transform.scale;
	transformComponent.local = sNode->transform.local;
	transformComponent.world = sNode->transform.world;
}
