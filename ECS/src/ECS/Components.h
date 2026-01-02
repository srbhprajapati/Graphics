#pragma once

#include "entt.hpp"
#include <windows.h>
#include <glm.hpp>
#include <gtc/quaternion.hpp>


struct RelationshipComponent {

	entt::entity parent = entt::null;
	entt::entity firstChild = entt::null;
	entt::entity nextSibling = entt::null;
	entt::entity prevSibling = entt::null;

};


struct TransformComponent
{
	glm::vec3 position;
	glm::fquat rotation;
	glm::vec3 scale;

	glm::mat4 local;
	glm::mat4 world;
};


struct MeshComponent
{
	MeshHandle mesh;
	unsigned int materialIndex;
};

struct CameraComponent
{
	unsigned int cameraIndex;
	glm::mat4 projectionMatrix;
};

struct LightComponent
{
	unsigned int lightIndex;
};


struct MaterialComponent {

	GLuint shaderID;

	std::string Name;

	glm::vec3 AmbientColor;
	glm::vec3 DiffuseColor;
	glm::vec3 TransparentColor;
	glm::vec3 SpecularColor;
	glm::vec3 ReflectionColor;
	glm::vec3 EmissiveColor;

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
