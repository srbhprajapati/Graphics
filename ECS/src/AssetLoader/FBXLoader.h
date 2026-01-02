#pragma once

#include <fbxsdk.h>
#include <string>
#include <memory>
#include <unordered_map>
#include "Common/SceneData.h"


class FBXLoader
{

public:

	static void LoadFBXFile(std::string filePath, SceneData* scene);
	
private:

	static void ParseScene(FbxScene* fbxScene, SceneData* scene, std::unordered_map<FbxSurfaceMaterial*, uint32_t>& map);
	static void ParseNode(FbxNode* fbxNode, SceneData* scene, int nodeLevel, SceneNode* parent, std::unordered_map<FbxSurfaceMaterial*, uint32_t>& map);
	static void ParseMaterials(FbxScene* fbxScene, SceneData* scene, std::unordered_map<FbxSurfaceMaterial*, uint32_t>& map, uint32_t& materialIndex);
	
	//static void GetEmissiveProperty(FbxSurfaceMaterial* material, MaterialData& mData);
	//static void GetAmbientProperty(FbxSurfaceMaterial* material, MaterialData& mData);
	//static void GetDiffuseProperty(FbxSurfaceMaterial* material, MaterialData& mData);
	//static void GetSpecularProperty(FbxSurfaceMaterial* material, MaterialData& mData);

	static SceneNode* ParseMesh(FbxNode* node, SceneNode* parent, std::unordered_map<FbxSurfaceMaterial*, uint32_t>& map);
	static SceneNode* ParseLight(FbxNode* node, SceneNode* parent);
	static SceneNode* ParseCamera(FbxNode* node, SceneNode* parent);

	static FbxVector4 GetNormal(FbxMesh* mesh, int polygonIndex, int vertexIndex, int normalLayer = 0);
	static FbxVector2 GetUV(FbxMesh* mesh, int polygonIndex, int vertexIndex, int uvLayer = 0);

	static void GetProperty(FbxSurfaceMaterial* material, const char* propertyName, Color& col);
	static void GetProperty(FbxSurfaceMaterial* material, const char* propertyName, float& factor);
	static void GetProperty(FbxSurfaceMaterial* material, const char* propertyName, std::string& path);
	static void GetProperty(FbxSurfaceMaterial* material, const char* propertyName, bool& val);


	static glm::mat4 ConvertFbxToGLM(FbxAMatrix mat);

};

