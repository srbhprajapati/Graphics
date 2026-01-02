#pragma once


#include <glad/glad.h>
#include "Common/SceneData.h"
#include <vector>
#include "Material.h"


class Mesh {

public:
	
	Mesh(std::vector<Vertex> vert, std::vector<Normal> norm, std::vector<UV> uv, std::vector<unsigned int> ind);
	~Mesh();

	void Render(const glm::mat4& viewProjectionTransform, const glm::mat4& modelTransform);

	void SetMaterial(const Material* mat) { material = mat; }
	

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

	const Material* material;

};