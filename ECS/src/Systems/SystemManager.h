#pragma once

#include "System.h"
#include <vector>
#include <memory>

class SystemManager
{
public:

	SystemManager();
	~SystemManager();

	template<typename T,  typename ... Args>
	T& AddSystem(Args&& ... args)
	{
		auto system = std::make_unique<T>(std::forward<Args>(args)...);
		T& ref = *system;
		systems.emplace_back(std::move(system));
		return ref;
	}

	template<typename T>
	T& GetSystem()
	{
		for (auto& system : systems)
		{
			if (auto ptr = dynamic_cast<T*>(system.get()))
			{
				return *ptr;
			}

		}

		assert(false && "Requested system not found");
		std::terminate();
	}

private:

	std::vector<std::unique_ptr<System>> systems;
};