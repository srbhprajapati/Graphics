#pragma once

#include <vector>

class SimpleMeshRenderer
{
public:
	SimpleMeshRenderer(std::vector<float>* vertexArray, std::vector<unsigned int>* indexArray);
	~SimpleMeshRenderer();


private:

	std::vector<float>* _PositionArray;
	std::vector<unsigned int>* _IndexArray;

};

