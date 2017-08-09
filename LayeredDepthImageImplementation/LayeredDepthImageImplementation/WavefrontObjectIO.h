#pragma once

#include <string>
#include <vector>


class WavefrontObjectIO
{
public:

	static bool LoadObjectFile(std::string objectFile, std::vector<float>* vertexArray, std::vector<unsigned int>* indexArray);
};

