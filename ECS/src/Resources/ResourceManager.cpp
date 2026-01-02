#include "ResourceManager.h"

MeshHandle ResourceManager::CreateMesh(std::vector<Vertex> vert, std::vector<Normal> norm, std::vector<UV> uv, std::vector<unsigned int> ind, uint32_t matId)
{
	auto mesh = std::make_unique<Mesh>(vert, norm, uv, ind);
	mesh->SetMaterial(materials[matId].get());
	
	MeshHandle id = nextMeshHandle++;
	meshes.emplace(id, std::move(mesh));

	return id;
}


MaterialHandle ResourceManager::CreateMaterial(MaterialData& data)
{
	auto material = std::make_unique<Material>(data);
	materials.emplace(data.ID, std::move(material));
	
	return data.ID;
}