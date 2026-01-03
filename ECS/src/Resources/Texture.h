#pragma once
#include <string>
#include <glad/glad.h>


class Texture
{
public:

	Texture(std::string texpath);

	GLuint GetTextureId() { return textureId; }

private:

	void Initiaize(std::string texpath);

	GLuint CreateGLTextureFromDDS(std::string texPath);

	GLuint textureId = 0;

};