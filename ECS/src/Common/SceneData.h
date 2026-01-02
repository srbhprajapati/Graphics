#pragma once

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <glm.hpp>
#include <gtc/quaternion.hpp>

struct Color {
	float r;
	float g;
	float b;
};

struct Vertex {
	float x;
	float y;
	float z;
};

struct Normal {
	float x;
	float y;
	float z;
};

struct UV {
	float u;
	float v;
};

struct Transform {
	glm::vec3 position;
	glm::fquat rotation;
	glm::vec3 scale;

	glm::mat4 local;
	glm::mat4 world;
};

struct SceneNode
{
	unsigned int nodeId;
	std::string nodeName;
	std::vector<std::unique_ptr<SceneNode>> children;
	std::string nodeType;

	Transform transform;
	
	virtual ~SceneNode() = default;
};


struct MeshNode : public SceneNode {
	std::vector<Vertex> vertices;
	std::vector<Normal> normals;
	std::vector<UV> uvs;
	std::vector<unsigned int> indices;

	uint32_t materialIndex;
};


struct LightNode : public SceneNode {
	unsigned int lightIndex;
};

struct CameraNode : public SceneNode {
	unsigned int cameraIndex;
	glm::mat4 projectionMatrix;
};



struct MaterialData {

	uint32_t ID;

	std::string Name;
	std::string ShadingModel;
	bool IsMultiLayer;

	Color AmbientColor;
	Color DiffuseColor;
	Color TransparentColor;
	Color SpecularColor;
	Color ReflectionColor;
	Color EmissiveColor;
	Color DisplacementColor;
	Color VectorDisplacementColor;

	float AmbientFactor;
	float DiffuseFactor;
	float SpecularFactor;
	float EmissiveFactor;
	float TransparencyFactor;
	float ReflectionFactor;
	float BumpFactor;
	float DisplacementFactor;
	float VectorDisplacementFactor;

	std::string DiffuseTexture = "";
	std::string SpecularTexture = "";
	std::string EmissiveTexture = "";
	std::string NormalMapTexture = "";
	std::string BumpMapTexture = "";

	float Shininess;
	float Opacity;
	float Reflectivity;
};


struct SceneData
{	
	std::unique_ptr<SceneNode> rootNode;
	std::vector<MaterialData> Materials;

	unsigned int latestNodeIndex = 0;
};

