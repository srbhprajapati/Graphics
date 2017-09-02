#include "stdafx.h"
#include "SimpleMeshRenderer.h"


SimpleMeshRenderer::SimpleMeshRenderer(std::vector<float>* vertexArray, std::vector<unsigned int>* indexArray)
	: _PositionArray(vertexArray),
	_IndexArray(indexArray),
	_IsValid(false)
{
	if (_PositionArray != nullptr && _IndexArray != nullptr && _PositionArray->size() != 0 && _IndexArray->size() != 0)
	{

		for (int i = 0; i < _PositionArray->size(); i++)
		{
			(*_PositionArray)[i] /= 50.0;
		}
		InitMeshBufferObject();
	}
}


SimpleMeshRenderer::~SimpleMeshRenderer()
{
	//deallocated the resources

}

bool  SimpleMeshRenderer::InitMeshBufferObject()
{
	//Create Vertex Buffer Object
	glGenBuffers(1, &_MeshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _MeshVBO);
	glBufferData(GL_ARRAY_BUFFER, _PositionArray->size() * sizeof(float), &(*_PositionArray)[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Create Element Buffer Object
	glGenBuffers(1, &_MeshEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _MeshEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _IndexArray->size() * sizeof(GLuint), &(*_IndexArray)[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Create and Bind the Vertex Array Object
	glGenVertexArrays(1, &_MeshVAO);
	glBindVertexArray(_MeshVAO);

	//Bind Vertex and Index Data
	glBindBuffer(GL_ARRAY_BUFFER, _MeshVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _MeshEBO);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			3 * sizeof(float),  // stride
			0					// array buffer offset
			);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

void SimpleMeshRenderer::Draw()
{
	glBindVertexArray(_MeshVAO);
	glDrawElements(GL_TRIANGLES, _IndexArray->size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
