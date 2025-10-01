#include "SceneGraph.h"
#include "Entity.h"
#include "Components.h"

SceneGraph::SceneGraph(SceneData& sData)
{
	Initialize(sData);
}

void SceneGraph::Initialize(SceneData& sData)
{
	ParseSceneNode(sData.rootNode, Entity(entt::null, this));
}


Entity SceneGraph::CreateEntity()
{
	auto eid = SceneRegistry.create();
	return Entity(eid, this);
}


Entity SceneGraph::GetEntity(entt::entity e)
{
	return Entity(e, this);
}

void SceneGraph::DeleteEntity(Entity e)
{
	SceneRegistry.destroy(e.GetID());
}

Entity SceneGraph::ParseSceneNode(std::unique_ptr<SceneNode>& node, Entity& parent)
{
	Entity nodeEntity = CreateEntity();
	
	//Transform Component
	nodeEntity.AddComponent<TransformComponent>(node->localTransform.matrix);
	
	
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
	}

	//Camera Component
	if (auto cameraNode = dynamic_cast<const CameraNode*>(node.get()))
	{
		AddCameraComponent(nodeEntity, cameraNode);
	}

	//Light Component
	if (auto lightNode = dynamic_cast<const LightNode*>(node.get()))
	{
		AddLightComponent(nodeEntity, lightNode);
	}

	return nodeEntity;
}

void SceneGraph::AddMeshComponent(Entity& node, const MeshNode* sNode)
{
	auto& meshComponent = node.AddComponent<MeshComponent>();

	meshComponent.indices = sNode->indices;
	meshComponent.materialIndex = sNode->materialIndex;
	meshComponent.normals = sNode->normals;
	meshComponent.uvs = sNode->uvs;
	meshComponent.vertices = sNode->vertices;
}

void SceneGraph::AddCameraComponent(Entity& node, const CameraNode* sNode)
{
	auto& cameraComponent = node.AddComponent<CameraComponent>();

	cameraComponent.cameraIndex = sNode->cameraIndex;
}

void SceneGraph::AddLightComponent(Entity& node, const LightNode* sNode)
{
	auto& lightComponent = node.AddComponent<LightComponent>();

	lightComponent.lightIndex = sNode->lightIndex;
}

