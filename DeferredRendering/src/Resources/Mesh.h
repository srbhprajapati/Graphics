#pragma once

#include "Common/SceneData.h"
#include <vector>
#include <glad/glad.h>


class Mesh {

public:
	
	Mesh(std::vector<Vertex> vert, std::vector<Normal> norm, std::vector<UV> uv, std::vector<unsigned int> ind);
	~Mesh();

private:

	void InitMesh();

	std::vector<Vertex> vertices;
	std::vector<Normal> normals;
	std::vector<UV> uvs;
	std::vector<unsigned int> indices;

	GLuint vao;
	GLuint vbo;
	GLuint nbo;
	GLuint uvbo;
	GLuint ebo;


};