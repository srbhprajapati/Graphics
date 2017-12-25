#pragma once

#include <stdlib.h>
#include <GL\glew.h>
#include <GL\glut.h>


class GLUtilities
{


	public:

		struct FrameBufferDataStructure
		{
			GLuint frameBufferId;
			GLuint renderBufferId;
			GLuint firstTextureId;
			GLuint secondTextureId;
			GLuint normalTextureId;
			int width;
			int height;
		};


		static bool checkFrameBufferStatus();
		static bool checkError();
		static GLuint initTexture(int width, int height, float* data);
		static bool initFrameBufferObject(FrameBufferDataStructure* frameBuffer, float* firstTextureData, float* secondTextureData, float* normalTextureData, int width, int height);
};

