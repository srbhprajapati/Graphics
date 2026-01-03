#pragma once
#include "Common/SceneData.h"
#include "PbrShader.h"
#include "Texture.h"


class Material
{

public:
	
	Material(MaterialData& mData);

	const Shader* GetShader() const { return shader.get(); }

	void BindTextures() const;
	void UnbindTextures() const;

private:

	
	uint32_t id;

	std::string name;

	Color ambientColor;
	Color diffuseColor;
	Color transparentColor;
	Color specularColor;
	Color reflectionColor;
	Color emissiveColor;

	float ambientFactor;
	float diffuseFactor;
	float transparencyFactor;
	float reflectionFactor;

	std::unique_ptr<Texture> diffuseTexture;
	std::unique_ptr<Texture> specularTexture;
	std::unique_ptr<Texture> emissiveTexture;
	std::unique_ptr<Texture> normalTexture;

	float shininess;
	float opacity;
	float reflectivity;

	std::unique_ptr<PbrShader> shader;
	

};