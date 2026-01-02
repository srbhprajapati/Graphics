#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vert, std::vector<Normal> norm, std::vector<UV> uv, std::vector<unsigned int> ind):
	vertices(std::move(vert)),
	normals(std::move(norm)),
	uvs(std::move(uv)),
	indices(std::move(ind))
{
	InitMesh();
}


Mesh::~Mesh()
{

}


void Mesh::InitMesh()
{
	//Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Position
	glCreateBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	//Normal
	glCreateBuffers(1, &nbo);
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normal) * normals.size(), &normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), (void*)0);

	//UVs
	glCreateBuffers(1, &uvbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(UV) * uvs.size(), &uvs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(UV), (void*)0);

	//Indices
	glCreateBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}


void Mesh::Render(const glm::mat4& viewProjectionTransform, const glm::mat4& modelTransform)
{
	material->GetShader()->Bind();
	material->GetShader()->SetUniformMat4("model", modelTransform);
	material->GetShader()->SetUniformMat4("viewProjection", viewProjectionTransform);

	glBindVertexArray(vao);
	glDrawElements(
		GL_TRIANGLES,
		indices.size(),
		GL_UNSIGNED_INT,
		nullptr
	);

	material->GetShader()->UnBind();
}