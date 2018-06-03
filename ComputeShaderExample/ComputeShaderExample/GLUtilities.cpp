#include "stdafx.h"
#include "GLUtilities.h"
#include <iostream>


bool GLUtilities::initFrameBufferObject(FrameBufferDataStructure* frameBuffer, float* firstTextureData, float* secondTextureData, float* normalTextureData, int width, int height)
{
	frameBuffer->width = width;
	frameBuffer->height = height;
	frameBuffer->firstTextureId = initTexture(frameBuffer->width, frameBuffer->height, firstTextureData);
	frameBuffer->secondTextureId = initTexture(frameBuffer->width, frameBuffer->height, secondTextureData);
	frameBuffer->normalTextureId = initTexture(frameBuffer->width, frameBuffer->height, normalTextureData);

	glGenFramebuffers(1, &frameBuffer->frameBufferId); // frame buffer id
	glGenRenderbuffers(1, &frameBuffer->renderBufferId); // render buffer id
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->frameBufferId);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBuffer->firstTextureId, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, frameBuffer->secondTextureId, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, frameBuffer->normalTextureId, 0);

	// initialize depth renderbuffer
	glBindRenderbuffer(GL_RENDERBUFFER, frameBuffer->renderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, frameBuffer->width, frameBuffer->height);
	// attach renderbuffer to framebuffer depth buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, frameBuffer->renderBufferId);

	bool isFrameBufferComplete = checkFrameBufferStatus();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return isFrameBufferComplete;
}


bool GLUtilities::checkError()
{
	GLenum error = glGetError();

	if (error != GL_NO_ERROR)
	{
		std::cout << "Error has been occured : " << gluErrorString(error) << std::endl;
		return false;
	}

	return true;
}

GLuint GLUtilities::initTexture(int width, int height, float* data)
{
	GLuint texid;
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, data); // NULL = Empty texture

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // FBO safe
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
	return texid;
}


bool GLUtilities::checkFrameBufferStatus()
{
	GLenum status;
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		std::cout << "Framebuffer Complete" << std::endl;
		return true;

	case GL_FRAMEBUFFER_UNSUPPORTED:
		std::cout << "Framebuffer unsuported" << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		std::cout << "GL_FRAMEBUFFER_MISSING_ATTACHMENT" << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		std::cout << "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS" << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		std::cout << "GL_FRAMEBUFFER_INCOMPLETE_FORMATS" << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		std::cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		std::cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << std::endl;
		return false;

	default:
		std::cout << "Framebuffer error" << std::endl;
		return false;
	}
}
