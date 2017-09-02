#pragma once

#include <GL/glew.h>
#include <vector>


class SimpleMeshRenderer
{

public:

	SimpleMeshRenderer(std::vector<float>* vertexArray, std::vector<unsigned int>* indexArray);
	~SimpleMeshRenderer();


	void Draw();

private:

	bool  InitMeshBufferObject();

	std::vector<float>* _PositionArray;
	std::vector<unsigned int>* _IndexArray;

	GLuint _MeshVBO;
	GLuint _MeshVAO;
	GLuint _MeshEBO;

	bool _IsValid;
};

