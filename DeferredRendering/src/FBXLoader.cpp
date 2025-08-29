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
	ParseMaterials(fbxScene, scene);

	fbxScene->Destroy();
	importer->Destroy();
	ioSettings->Destroy();
	sdkManager->Destroy();
}




void FBXLoader::ParseScene(FbxScene* fbxScene, SceneData* scene)
{
	FbxNode* rootNode = fbxScene->GetRootNode();

	scene->rootNode = std::make_unique<SceneNode>();

	scene->rootNode->nodeId = scene->latestNodeIndex++;
	scene->rootNode->nodeName = "RootNode";
	scene->rootNode->nodeType = "Unknown";

	if (rootNode)
	{
		for (int i = 0; i < rootNode->GetChildCount(); i++)
		{
			FbxNode* childNode = rootNode->GetChild(i);
			ParseNode(childNode, scene, 1, scene->rootNode.get());
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


void FBXLoader::ParseMaterials(FbxScene* fbxScene, SceneData* scene)
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

	FbxMesh* fbxMesh = node->GetMesh();

	int polygonCount = fbxMesh->GetPolygonCount();
	unsigned int vertexOffset = 0;

	for (int poly = 0; poly < polygonCount; ++poly) {
		
		int polySize = fbxMesh->GetPolygonSize(poly);
		
		//Only Polygon of size 3 and 4 supported (More can be supported later on)
		if (polySize == 3)
		{
			for (int vert = 0; vert < polySize; ++vert) {
				int cpIndex = fbxMesh->GetPolygonVertex(poly, vert);


				
				FbxVector4 position = fbxMesh->GetControlPointAt(cpIndex);
				FbxVector4 normal = GetNormal(fbxMesh, poly, vert);
				FbxVector2 uv = GetUV(fbxMesh, poly, vert);

				mesh->vertices.push_back({ (float)position[0], (float)position[1], (float)position[2] });
				mesh->normals.push_back({ (float)normal[0], (float)normal[1], (float)normal[2] });
				mesh->uvs.push_back({ (float)uv[0], (float)uv[1] });
				mesh->indices.push_back(vertexOffset++);
			}
		}
		else if (polySize == 4)
		{
			int vertArray[] = { 0,1,2,0,2,3 };

			for (int i = 0; i < 6; i++) {
				int vert = vertArray[i];
				int cpIndex = fbxMesh->GetPolygonVertex(poly, vert);

				FbxVector4 position = fbxMesh->GetControlPointAt(cpIndex);
				FbxVector4 normal = GetNormal(fbxMesh, poly, vert);
				FbxVector2 uv = GetUV(fbxMesh, poly, vert);

				mesh->vertices.push_back({ (float)position[0], (float)position[1], (float)position[2] });
				mesh->normals.push_back({ (float)normal[0], (float)normal[1], (float)normal[2] });
				mesh->uvs.push_back({ (float)uv[0], (float)uv[1] });
				mesh->indices.push_back(vertexOffset++);
			}

		}
	}

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



// Helper function to get Normal per polygon vertex
FbxVector4 FBXLoader::GetNormal(FbxMesh* mesh, int polygonIndex, int vertexIndex, int normalLayer) {
	
	FbxVector4 normalValue(0.0f, 0.0f, 0.0f, 0.0f);
	if (mesh->GetElementNormalCount() > normalLayer) {
		auto* normalElement = mesh->GetElementNormal(normalLayer);
		if (normalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
			int cpIndex = mesh->GetPolygonVertex(polygonIndex, vertexIndex);
			int index = (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
				? cpIndex
				: normalElement->GetIndexArray().GetAt(cpIndex);
			normalValue = normalElement->GetDirectArray().GetAt(index);
		}
		else if (normalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
			int polyVertIndex = polygonIndex * 3 + vertexIndex; // triangles
			int index = (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
				? polyVertIndex
				: normalElement->GetIndexArray().GetAt(polyVertIndex);
			normalValue = normalElement->GetDirectArray().GetAt(index);
		}
	}
	return normalValue;
}


// Helper function to get UV per polygon vertex
FbxVector2 FBXLoader::GetUV(FbxMesh* mesh, int polygonIndex, int vertexIndex, int uvLayer) {
	FbxVector2 uvValue;
	if (mesh->GetElementUVCount() > uvLayer) {
		auto* uvElement = mesh->GetElementUV(uvLayer);
		if (uvElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
			int cpIndex = mesh->GetPolygonVertex(polygonIndex, vertexIndex);
			int index = (uvElement->GetReferenceMode() == FbxGeometryElement::eDirect)
				? cpIndex
				: uvElement->GetIndexArray().GetAt(cpIndex);
			uvValue = uvElement->GetDirectArray().GetAt(index);
		}
		else if (uvElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
			int polyVertIndex = polygonIndex * 3 + vertexIndex; // assume triangles
			int index = (uvElement->GetReferenceMode() == FbxGeometryElement::eDirect)
				? polyVertIndex
				: uvElement->GetIndexArray().GetAt(polyVertIndex);
			uvValue = uvElement->GetDirectArray().GetAt(index);
		}
	}
	return uvValue;
}
