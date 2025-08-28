#pragma once

#include <vector>
#include <string>
#include <map>
#include <memory>


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

struct Transform {
	float matrix[16];
};

struct SceneNode
{
	unsigned int nodeId;
	std::string nodeName;
	std::vector<std::unique_ptr<SceneNode>> children;
	std::string nodeType;

	Transform localTransform;
	Transform globalTransform;
};


struct MeshNode : public SceneNode {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	unsigned int materialIndex;
};


struct LightNode : public SceneNode {
	unsigned int lightIndex;
};

struct CameraNode : public SceneNode {
	unsigned int cameraIndex;
};



struct Material {

	std::string Name;

	Color AmbientColor;
	Color DiffuseColor;
	Color TransparentColor;
	Color SpecularColor;
	Color ReflectionColor;
	Color EmissiveColor;

	float AmbientFactor;
	float DiffuseFactor;
	float TransparencyFactor;
	float ReflectionFactor;

	std::string DiffuseTexture = "";
	std::string SpecularTexture = "";
	std::string EmissiveTexture = "";

	float Shininess;
	float Opacity;
	float Reflectivity;
};


struct SceneData
{	
	SceneNode rootNode;
	std::map<std::string, Material> Materials;

	unsigned int latestNodeIndex = 0;
};

