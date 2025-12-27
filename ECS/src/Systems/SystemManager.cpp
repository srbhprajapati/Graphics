#include "SystemManager.h"


SystemManager::SystemManager()
{

}

SystemManager::~SystemManager()
{

}

void SystemManager::UpdateAll(float deltaTime)
{
	for (auto& system : systems)
	{
		system->Update(deltaTime);
	}
}