#include "RenderSystem.h"
#include "ECS/Components.h"

RenderSystem::RenderSystem(entt::registry& registry, ResourceManager& resManager): reg(registry),
resManager(resManager)
{}


RenderSystem::~RenderSystem()
{
}

void RenderSystem::Update(float deltaTime)
{
	auto views = reg.view<TransformComponent, MeshComponent>();

	for (auto entity : views)
	{
		const auto& transform = views.get<TransformComponent>(entity);
		const auto& mesh = views.get<MeshComponent>(entity);

		Mesh* meshref = resManager.GetMesh(mesh.mesh);

		glBindVertexArray(meshref->GetVAO());
		glDrawElements(
			GL_TRIANGLES,
			meshref->GetIndexCount(),
			GL_UNSIGNED_INT,
			nullptr
		);
	}

	glBindVertexArray(0);
}
