#include "FBXLoader.h"

#include <iostream>


void FBXLoader::LoadFBXFile(std::string filePath, SceneData* scene)
{
	FbxManager* sdkManager = FbxManager::Create();
	FbxIOSettings* ioSettings = FbxIOSettings::Create(sdkManager, IOSROOT);

	FbxImporter* importer = FbxImporter::Create(sdkManager, "DefaultImporter");
	
	if (!importer->Initialize(filePath.c_str(), -1, sdkManager->GetIOSettings()))
	{
		std::cerr <<"Error in Initializing FBX Importer : " 
			<< importer->GetStatus() << " -- "
			<< importer->GetStatus().GetErrorString() << std::endl;
			
		return ;
	}

	FbxScene* fbxScene = FbxScene::Create(sdkManager, "DefaultScene");
	importer->Import(fbxScene);
	


	ParseScene(fbxScene, scene);
	//GetMaterials(fbxScene, scene);

	fbxScene->Destroy();
	importer->Destroy();
	ioSettings->Destroy();
	sdkManager->Destroy();
}




void FBXLoader::ParseScene(FbxScene* fbxScene, SceneData* scene)
{
	FbxNode* rootNode = fbxScene->GetRootNode();

	if (rootNode)
	{
		for (int i = 0; i < rootNode->GetChildCount(); i++)
		{
			FbxNode* childNode = rootNode->GetChild(i);
			ParseNode(childNode, scene, 1, nullptr);
		}
	}
}


void FBXLoader::ParseNode(FbxNode* fbxNode, SceneData* scene, int nodeLevel, SceneNode* parent)
{
	SceneNode* node = nullptr;


	// Get the node attribute (mesh, light, camera, etc.)
	FbxNodeAttribute* attr = fbxNode->GetNodeAttribute();
	if (attr) {
		switch (attr->GetAttributeType()) {
		case FbxNodeAttribute::eMesh:
			node = ParseMesh(fbxNode, parent);
			
			break;
		case FbxNodeAttribute::eLight:
			node = ParseLight(fbxNode, parent);
			node->nodeType = "Light";
			break; 
		case FbxNodeAttribute::eCamera:
			node = ParseLight(fbxNode, parent);
			node->nodeType = "Camera";
			break;
		default:
			parent->children.push_back(std::make_unique<SceneNode>());
			node = parent->children.back().get();
			node->nodeType = "Unknown";
			break;
		}
	}

	node->nodeId = scene->latestNodeIndex++;
	node->nodeName = fbxNode->GetName();;

	int newNodeLevel = nodeLevel + 1;
	for (int i = 0; i < fbxNode->GetChildCount(); i++)
	{
		FbxNode* childNode = fbxNode->GetChild(i);
		ParseNode(childNode, scene, newNodeLevel, node);
	}
}


void FBXLoader::GetMaterials(FbxScene* fbxScene, SceneData* scene)
{
	int materialCount = fbxScene->GetSrcObjectCount<FbxSurfaceMaterial>();
	for (int i = 0; i < materialCount; ++i)
	{
		Material sceneMaterial;

		// Retrieve the material at the current index
		FbxSurfaceMaterial* material = fbxScene->GetSrcObject<FbxSurfaceMaterial>(i);
		sceneMaterial.Name = material->GetName();

		FbxProperty prop = material->GetFirstProperty();
		while(prop.IsValid())
		{
			
			if (!prop.HasDefaultValue(prop))
			{
				FbxString propertyName = prop.GetName();
				std::cout << "	PropertyName : " << propertyName << std::endl;

				if (propertyName == "AmbientColor")	
				{
					GetProperty(prop, sceneMaterial.AmbientColor);
				}
				else if (propertyName == "DiffuseColor")
				{
					GetProperty(prop, sceneMaterial.DiffuseTexture);
					GetProperty(prop, sceneMaterial.DiffuseColor);
				}
				else if (propertyName == "DiffuseFactor")
				{
					GetProperty(prop, sceneMaterial.DiffuseFactor);
				}
				else if (propertyName == "TransparentColor")
				{
					GetProperty(prop, sceneMaterial.TransparentColor);
				}
				else if (propertyName == "TransparencyFactor")
				{
					GetProperty(prop, sceneMaterial.TransparencyFactor);
				}
				else if (propertyName == "SpecularColor")
				{
					GetProperty(prop, sceneMaterial.SpecularColor);
					GetProperty(prop, sceneMaterial.SpecularTexture);
				}
				else if (propertyName == "ReflectionColor")
				{
					GetProperty(prop, sceneMaterial.ReflectionColor);
				}
				else if (propertyName == "ReflectionFactor")
				{
					GetProperty(prop, sceneMaterial.ReflectionFactor);
				}
				else if (propertyName == "Emissive")
				{
					GetProperty(prop, sceneMaterial.EmissiveColor);
					GetProperty(prop, sceneMaterial.EmissiveTexture);
				}
				else if (propertyName == "Shininess")
				{
					GetProperty(prop, sceneMaterial.Shininess);
				}
				else if (propertyName == "Opacity")
				{
					GetProperty(prop, sceneMaterial.Opacity);
				}
				else if (propertyName == "Reflectivity")
				{
					GetProperty(prop, sceneMaterial.Reflectivity);
				}
			}
			prop = material->GetNextProperty(prop);
		}
		scene->Materials.emplace(sceneMaterial.Name, (std::move(sceneMaterial)));
	}
}



void FBXLoader::GetProperty(FbxProperty& prop, float& factor)
{
	if (prop.GetPropertyDataType().GetType() == eFbxDouble)
	{
		FbxDouble fac = prop.Get<FbxDouble>();
		factor = fac;
	}

}

void FBXLoader::GetProperty(FbxProperty& prop, Color& col)
{

	if (prop.GetPropertyDataType().GetType() == eFbxDouble3)
	{
		FbxDouble3 color = prop.Get<FbxDouble3>();
		col.r = color[0];
		col.g = color[1];
		col.b = color[2];
	}
}

void FBXLoader::GetProperty(FbxProperty& prop, std::string& path)
{
	int textureCount = prop.GetSrcObjectCount<FbxFileTexture>();
	if (textureCount > 0)
	{
		FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(0);
		path = texture->GetRelativeFileName();
	}
}


SceneNode* FBXLoader::ParseMesh(FbxNode* node, SceneNode* parent)
{
	auto mesh = std::make_unique<MeshNode>();
	mesh->nodeType = "Mesh";

	parent->children.push_back(std::move(mesh));
	return parent->children.back().get();
}


SceneNode* FBXLoader::ParseLight(FbxNode* node, SceneNode* parent)
{
	auto light = std::make_unique<LightNode>();
	light->nodeType = "Light";

	parent->children.push_back(std::move(light));
	return parent->children.back().get();
}


SceneNode* FBXLoader::ParseCamera(FbxNode* node, SceneNode* parent)
{
	auto camera = std::make_unique<CameraNode>();
	camera->nodeType = "Camera";

	parent->children.push_back(std::move(camera));
	return parent->children.back().get();
}
