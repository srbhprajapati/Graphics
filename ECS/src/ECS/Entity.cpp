#include "Entity.h"
#include "SceneRegistry.h"

Entity::Entity(entt::entity id, SceneRegistry* scene) : id(id), scene(scene)
{
}

