#include "RenderSystem.h"
#include "ECS/Components.h"

RenderSystem::RenderSystem(entt::registry& registry, ResourceManager& resManager): reg(registry),
resManager(resManager)
{}


RenderSystem::~RenderSystem()
{
}

void RenderSystem::BeginFrame()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

}

void RenderSystem::Update(float deltaTime)
{
	auto views = reg.view<TransformComponent, MeshComponent>();
	auto cameras = reg.view<TransformComponent, CameraComponent>();


	const auto& cameraTransform = cameras.get<TransformComponent>(cameras.front()).world;
	const auto& cameraProjection = cameras.get<CameraComponent>(cameras.front()).projectionMatrix;
	angle += 0.01f * deltaTime;


	//Camera Rotational Effect
	auto updatedCameraTransform = cameraTransform * glm::mat4_cast(glm::angleAxis(glm::radians(angle), glm::vec3(0, 1, 0)));;
	glm::mat4 viewProjectionMatrix = cameraProjection * glm::inverse(updatedCameraTransform);

	for (auto entity : views)
	{
			const auto& transform = views.get<TransformComponent>(entity);
			const auto& mesh = views.get<MeshComponent>(entity);

			Mesh* meshref = resManager.GetMesh(mesh.mesh);

			meshref->Render(viewProjectionMatrix, transform.local /** glm::scale(glm::mat4(1.0f), transform.scale)*/);
	}

	glBindVertexArray(0);
}


void RenderSystem::EndFrame()
{

}