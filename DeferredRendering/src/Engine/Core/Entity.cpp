#include "Entity.h"
#include "SceneGraph.h"

Entity::Entity(entt::entity id, SceneGraph* scene) : id(id), scene(scene)
{
}

