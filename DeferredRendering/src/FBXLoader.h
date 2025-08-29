#pragma once

#include <fbxsdk.h>
#include <string>
#include <memory>
#include "Common/SceneData.h"


class FBXLoader
{

public:

	static void LoadFBXFile(std::string filePath, SceneData* scene);
	
private:

	static void ParseScene(FbxScene* fbxScene, SceneData* scene);
	static void ParseNode(FbxNode* fbxNode, SceneData* scene, int nodeLevel, SceneNode* parent);
	static void ParseMaterials(FbxScene* fbxScene, SceneData* scene);
	static SceneNode* ParseMesh(FbxNode* node, SceneNode* parent);
	static SceneNode* ParseLight(FbxNode* node, SceneNode* parent);
	static SceneNode* ParseCamera(FbxNode* node, SceneNode* parent);


	static FbxVector4 GetNormal(FbxMesh* mesh, int polygonIndex, int vertexIndex, int normalLayer = 0);
	static FbxVector2 GetUV(FbxMesh* mesh, int polygonIndex, int vertexIndex, int uvLayer = 0);
	static void GetProperty(FbxProperty& prop, Color& col);
	static void GetProperty(FbxProperty& prop, float& factor);
	static void GetProperty(FbxProperty& prop, std::string& path);
	//static std::string GetAttributeTypeName(FbxNodeAttribute::EType type);

};

