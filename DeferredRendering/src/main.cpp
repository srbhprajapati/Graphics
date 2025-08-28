#include <iostream>
#include <GLFW/glfw3.h>


#include "FBXLoader.h"
#include "Common/SceneData.h"

int main(int argc, char** argv)
{

	std::string fbxDatasetPath = "C:/Users/srbhp/Documents/Data/Bistro_v5_2/BistroInterior_Wine.fbx";

	SceneData sceneData;

	FBXLoader::LoadFBXFile(fbxDatasetPath, &sceneData);



	return 0;
}