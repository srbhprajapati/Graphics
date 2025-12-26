#pragma once

#include "entt.hpp"

class SceneRegistry;

class Entity
{
public :
	Entity(entt::entity id, SceneRegistry* scene);

	entt::entity GetID() { return id; }


	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		return scene->Registry.emplace<T>(id, std::forward<Args>(args)...);
	}

	template<typename T>
	void RemoveComponent()
	{
		scene->Registry.remove<T>(id);
	}

	template<typename T>
	bool HasComponent()
	{
		return scene->Registry.all_of<T>(id);
	}

	template<typename T>
	T& GetComponent()
	{
		return scene->Registry.get<T>(id);
	}



private:

	entt::entity id;
	SceneRegistry* scene;
};