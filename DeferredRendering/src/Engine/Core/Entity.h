#pragma once

#include "entt.hpp"

class SceneGraph;

class Entity
{
public :
	Entity(entt::entity id, SceneGraph* scene);

	entt::entity GetID() { return id; }


	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		return scene->SceneRegistry.emplace<T>(id, std::forward<Args>(args)...);
	}

	template<typename T>
	void RemoveComponent()
	{
		scene->SceneRegistry.remove<T>(id);
	}

	template<typename T>
	bool HasComponent()
	{
		return scene->SceneRegistry.all_of<T>(id);
	}

	template<typename T>
	T& GetComponent()
	{
		return scene->SceneRegistry.get<T>(id);
	}



private:

	entt::entity id;
	SceneGraph* scene;
};