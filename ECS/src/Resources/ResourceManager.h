#pragma once

#include <unordered_map>
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

using MeshHandle = uint32_t;
using ShaderHandle = uint32_t;
using TextureHandle = uint32_t;


class ResourceManager
{

public:

	MeshHandle CreateMesh(std::vector<Vertex> vert, std::vector<Normal> norm, std::vector<UV> uv, std::vector<unsigned int> ind);


private:

	std::unordered_map<MeshHandle, std::unique_ptr<Mesh>> meshes;
	std::unordered_map<ShaderHandle, std::unique_ptr<Shader>> shaders;
	std::unordered_map<TextureHandle, std::unique_ptr<Texture>> textures;

	MeshHandle nextMeshHandle = 1; //0 for invalid
	ShaderHandle nextShaderHandle = 1; //0 for invalid
	TextureHandle nextTextureHandle = 1; //0 for invalid

};