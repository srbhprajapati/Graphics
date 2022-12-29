#include <iostream>

#include <GL/glew.h>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <glut.h>

#include <vector>

#include "OBJ_Loader.h"


#define GL(func) func; CheckGLError(__FILE__, __LINE__);



GLuint vbo;
GLuint ibo;
GLuint vao;
int numIndices = 0;


GLuint programId;


void CheckGLError(const char* file, int line)
{
	GLint error = glGetError();

	char* errorString;
	switch (error)
	{
	case GL_INVALID_ENUM:                               errorString = (char*)("GL_INVALID_ENUM"); break;
	case GL_INVALID_VALUE:                              errorString = (char*)("GL_INVALID_VALUE"); break;
	case GL_INVALID_OPERATION:                          errorString = (char*)("GL_INVALID_OPERATION"); break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:              errorString = (char*)("GL_INVALID_FRAMEBUFFER_OPERATION"); break;
	case GL_OUT_OF_MEMORY:                              errorString = (char*)("GL_OUT_OF_MEMORY"); break;
	case GL_STACK_UNDERFLOW:                            errorString = (char*)("GL_STACK_UNDERFLOW"); break;
	case GL_STACK_OVERFLOW:                             errorString = (char*)("GL_STACK_OVERFLOW"); break;
	default:                                            errorString = (char*)("SOME_ERROR"); break;
	}

	if (error != GL_NO_ERROR)
	{
		std::cout << "Error occured at line : [" << line << "] in file : [" << file << "]. ERROR_STRING : " << errorString << std::endl;
	}
}



void LoadMesh()
{
	objl::Loader objLoader;
	objLoader.LoadFile("C:\\Users\\srbhp\\Documents\\Mesh Resources\\Models\\Bench\\Obj\\Bench_LowRes.obj");

	for (int i = 0; i < objLoader.LoadedMeshes.size(); i++)
	{
		std::cout << "Mesh " << i << " :  " << objLoader.LoadedMeshes[i].MeshName << std::endl;
	}



	std::vector<objl::Vertex> vertexbuffer = objLoader.LoadedMeshes[0].Vertices;

	std::vector<float> positionBuffer;

	//for (int i = 0; i < vertexbuffer.size(); i++)
	//{
	//	positionBuffer.push_back(vertexbuffer[i].Position.X);
	//	positionBuffer.push_back(vertexbuffer[i].Position.Y);
	//	positionBuffer.push_back(vertexbuffer[i].Position.Z);
	//}

	positionBuffer.push_back(-0.5f);
	positionBuffer.push_back(0.0f);
	positionBuffer.push_back(1.0f);
	positionBuffer.push_back(0.0f);
	positionBuffer.push_back(0.5f);
	positionBuffer.push_back(1.0f);
	positionBuffer.push_back(0.5f);
	positionBuffer.push_back(0.0f);
	positionBuffer.push_back(5.0f);



	glCreateBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, positionBuffer.size() * sizeof(float), &positionBuffer[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//std::vector<unsigned int> indexbuffer = objLoader.LoadedMeshes[0].Indices;
	
	std::vector<unsigned int> indexbuffer;
	indexbuffer.push_back(0);
	indexbuffer.push_back(1);
	indexbuffer.push_back(2);


	glCreateBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexbuffer.size() * sizeof(unsigned int), &indexbuffer[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	numIndices = indexbuffer.size();


}


void InitializeBuffers()
{
	//int numVertices = 3;
	//data = new float[3 * numVertices];

	//data[0] = -0.5f;
	//data[1] = 0.0f;
	//data[2] = 1.0f;
	//data[3] = 0.0f;
	//data[4] = 0.5f;
	//data[5] = 1.0f;
	//data[6] = 0.5f;
	//data[7] = 0.0f;
	//data[8] = 5.0f;



	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void CreateShaderProgram()
{

	const GLchar* vertexShaderSource = R"(
	#version 330
	layout(location = 0) in vec3 position;
	void main()
	{
		gl_Position = vec4(position.x, position.y, 0.0, 1.0);
	}
	)";

	const GLchar* fragmentShaderSource = R"(
    #version 330
    out vec4 outColor;
    void main()
    {
        outColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
	)";


	programId = glCreateProgram();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);

		std::cout << "Error In Compiling Vertex shader. Error Log : " << &errorLog[0] << std::endl;
	}


	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);

		std::cout << "Error In Compiling Fragment shader. Error Log : " << &errorLog[0] << std::endl;
	}

	glAttachShader(programId, vertexShader);
	glAttachShader(programId, fragmentShader);
	glLinkProgram(programId);


	GLint status;
	glGetProgramiv(programId, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::vector<GLchar> errorLog(infoLogLength+1);
		glGetProgramInfoLog(programId, infoLogLength, NULL, &errorLog[0]);
		
		std::cout << "Error In Linking Shaders to the Program. Error Log : " << &errorLog[0] << std::endl;
		
	}

}

void DisplayFunc()
{
	GLint boundVbo, boundIbo;

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(programId);
	glBindVertexArray(vao);


	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);


	glBindVertexArray(0);
	glUseProgram(0);


	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Basic Mesh Loading");

	GLenum initVal = glewInit();
	if (initVal != GLEW_OK)
	{
		const GLubyte* errStr = glewGetErrorString(initVal);
		std::cout << "Unable to Initalize GLEW. Err : " << errStr << std::endl;
	}


	LoadMesh();
	InitializeBuffers();
	CreateShaderProgram();

	glutDisplayFunc(DisplayFunc);
	glutMainLoop();

	return 0;
}

