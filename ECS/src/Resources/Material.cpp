#include "Material.h"

Material::Material(MaterialData& mData)
{
	id = mData.ID;
	ambientColor = mData.AmbientColor;
	diffuseColor = mData.DiffuseColor;
	transparentColor = mData.TransparentColor;
	specularColor = mData.SpecularColor;
	reflectionColor = mData.ReflectionColor;
	emissiveColor = mData.EmissiveColor;

	ambientFactor = mData.AmbientFactor;
	diffuseFactor = mData.DiffuseFactor;
	transparencyFactor = mData.TransparencyFactor;
	reflectionFactor = mData.ReflectionFactor;

	diffuseTexture  = std::make_unique<Texture>(mData.DiffuseTexture);
	specularTexture = std::make_unique<Texture>(mData.SpecularTexture);
	normalTexture = std::make_unique<Texture>(mData.NormalMapTexture);
	emissiveTexture = std::make_unique<Texture>(mData.EmissiveTexture);

	shader = std::make_unique<PbrShader>();

}


void Material::BindTextures() const
{
	shader->SetDiffuseMap(diffuseTexture->GetTextureId());
	shader->SetSpecularMap(specularTexture->GetTextureId());
	shader->SetNormalMap(normalTexture->GetTextureId());
	shader->SetEmissiveMap(emissiveTexture->GetTextureId());
}

void Material::UnbindTextures() const
{
	shader->SetDiffuseMap(0);
	shader->SetSpecularMap(0);
	shader->SetNormalMap(0);
	shader->SetEmissiveMap(0);
}


