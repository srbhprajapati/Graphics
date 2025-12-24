#include "ResourceManager.h"

MeshHandle ResourceManager::CreateMesh(std::vector<Vertex> vert, std::vector<Normal> norm, std::vector<UV> uv, std::vector<unsigned int> ind)
{
	auto mesh = std::make_unique<Mesh>(vert, norm, uv, ind);

	MeshHandle id = nextMeshHandle++;
	meshes.emplace(id, std::move(mesh));

	return id;
}