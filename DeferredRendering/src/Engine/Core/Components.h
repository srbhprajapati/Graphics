#pragma once

#include "entt.hpp"
#include <glm.hpp>

struct RelationshipComponent {

	entt::entity parent = entt::null;
	entt::entity firstChild = entt::null;
	entt::entity nextSibling = entt::null;
	entt::entity prevSibling = entt::null;

};


struct TransformComponent
{
	glm::mat4 matrix;
};


