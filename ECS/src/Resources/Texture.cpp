#include "Texture.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>


Texture::Texture(std::string path)
{
	Initiaize(path);
}

void Texture::Initiaize(std::string path)
{
	std::filesystem::path p(path);

	if (p.extension().string() == ".dds")
	{
		CreateGLTextureFromDDS(path);
	}
	else
	{
		std::cerr << "File Format Not Supported. File Path : " << path << std::endl;
	}
}


GLuint Texture::CreateGLTextureFromDDS(std::string texPath)
{
	std::ifstream file(texPath.c_str(), std::iostream::binary);

	if (!file.is_open())
	{
		std::cerr << "Failed to open DDS file : " << texPath << std::endl;
		return 0;
	}


	//check for DDS 
	char fileCode[4];
	file.read(fileCode, 4);
	if (strncmp(fileCode, "DDS ", 4) != 0)
	{
		std::cerr << "Not a DDS File : " << texPath << std::endl;
		return 0;
	}


	//Read DDS file header
	unsigned char header[124];
	file.read(reinterpret_cast<char*>(header), 124);

	unsigned int height			= *reinterpret_cast<unsigned int*>(&header[8]);
	unsigned int width			= *reinterpret_cast<unsigned int*>(&header[12]);
	unsigned int linearSize		= *reinterpret_cast<unsigned int*>(&header[16]);
	unsigned int mipMapCount	= *reinterpret_cast<unsigned int*>(&header[24]);
	unsigned int fourCC			= *reinterpret_cast<unsigned int*>(&header[80]);


	// Read data buffer
	unsigned int bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	std::vector<unsigned char> buffer(bufsize);
	file.read(reinterpret_cast<char*>(buffer.data()), bufsize);
	file.close();


	// Create OpenGL texture
	
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	unsigned int format;
	switch (fourCC) 
	{
		case 0x31545844: // "DXT1"
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case 0x33545844: // "DXT3"
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case 0x35545844: // "DXT5"
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		case 0x32495441:
			format = GL_COMPRESSED_RG_RGTC2;
			break;
		default:
			std::cerr << "Unsupported DDS format in " << texPath << std::endl;
			return 0;
	}

	unsigned int blockSize;
	switch (format) {
		case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT: blockSize = 8; break;
		default: blockSize = 16; break;
	}

	unsigned int offset = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned int level = 0; level < mipMapCount && (width > 0 && height > 0); ++level) 
	{
		unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer.data() + offset);

		offset += size;
		width = (width > 1) ? (width / 2) : 1;
		height = (height > 1) ? (height / 2) : 1;
	}

	// Set default sampling params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		mipMapCount > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	glBindTexture(GL_TEXTURE_2D, 0);

	return textureId;
}