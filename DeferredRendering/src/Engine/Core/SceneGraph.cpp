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
	
	nodeEntity.AddComponent<TransformComponent>(node->localTransform.matrix);
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

	return nodeEntity;
}