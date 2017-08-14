#include "stdafx.h"
#include "SimpleMeshRenderer.h"


SimpleMeshRenderer::SimpleMeshRenderer(std::vector<float>* vertexArray, std::vector<unsigned int>* indexArray)
	: _PositionArray(vertexArray),
	_IndexArray(indexArray)
{
}


SimpleMeshRenderer::~SimpleMeshRenderer()
{
}
