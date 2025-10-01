#include "Material.h"

Material::Material(MaterialData& mData)
{
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
	emissiveTexture = std::make_unique<Texture>(mData.EmissiveTexture);

	shader = std::make_unique<PbrShader>();
}

void Material::InitMaterial()
{

}





