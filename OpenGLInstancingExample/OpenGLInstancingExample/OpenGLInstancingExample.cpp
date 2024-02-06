
#include <iostream>
#include <vector>
#include <string>


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800


GLuint vao, vbo, programId, instanceVbo;

float quadVertices[30] = {
	// positions     // colors
	-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
	-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,
	 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,

	-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
	 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
	 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
};


GLuint CreateShader(const GLchar* shaderSource, GLuint shaderType)
{
	GLuint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &shaderSource, NULL);
	glCompileShader(shaderId);

	GLint status;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::vector<GLchar*> errorLog(infoLogLength);
		glGetShaderInfoLog(shaderId, infoLogLength, &infoLogLength, errorLog[0]);

		std::string shaderTypeString;
		if (shaderType == GL_VERTEX_SHADER) shaderTypeString = "GL_VERTEX_SHADER";
		else if (shaderType == GL_FRAGMENT_SHADER) shaderTypeString = "GL_FRAGMENT_SHADER";
		else if (shaderType == GL_GEOMETRY_SHADER) shaderTypeString = "GL_GEOMETRY_SHADER";
		else if (shaderType == GL_COMPUTE_SHADER) shaderTypeString = "GL_COMPUTE_SHADER";

		std::cout << "Error in Compiling Shader. Shader Type : " << shaderTypeString << "  || Error : " << &errorLog[0] << std::endl;
		return 0;
	}
	return shaderId;
}




GLuint CreateProgram(std::vector<GLuint> shaders)
{
	GLuint programId = glCreateProgram();

	for (GLuint shader : shaders)
	{
		glAttachShader(programId, shader);
	}
	glLinkProgram(programId);

	GLint status;
	glGetProgramiv(programId, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::vector<GLchar*> errorLog(infoLogLength);
		glGetProgramInfoLog(programId, infoLogLength, &infoLogLength, errorLog[0]);

		std::cout << "Error in Linking Program. Error : " << &errorLog[0] << std::endl;
		return 0;
	}
	return programId;
}


void CreateVertexBuffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	std::vector<glm::vec2> offsets(100);
	GLfloat offsetGL[200];
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			offsets[i * 10 + j] = glm::vec2(i * 0.2f - 0.9f, j * 0.2f - 0.9f);
			offsetGL[2 * (i * 10 + j) + 0] = i * 0.2f - 0.9f;
			offsetGL[2 * (i * 10 + j) + 1] = j * 0.2f - 0.9f;

		}
	}



	glGenBuffers(1, &instanceVbo);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
	glBufferData(GL_ARRAY_BUFFER, 200 * sizeof(GLfloat), offsetGL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2* sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glVertexAttribDivisor(2, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void SetUniform()
{
	std::vector<glm::vec2> offsets(100);

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			offsets[i * 10 + j] = glm::vec2(i * 0.2f - 0.9f, j * 0.2f - 0.9f);
		}
	}

	glUseProgram(programId);
	glUniform2fv(glGetUniformLocation(programId, "offsets"), 100, &offsets[0].x);
	glUseProgram(0);
}


void CreateInstancedProgram()
{
	const GLchar* vertexShaderSource = R"(
	#version 430
	layout(location = 0) in vec2 position;
	layout(location = 1) in vec3 color;
	layout(location = 2) in vec2 offset;

	out vec3 colorOut;

	//uniform vec2 offsets[100];

	void main()
	{
		//vec2 offset = offsets[gl_InstanceID];
		gl_Position = vec4(position+offset, 0.0f, 1.0f);
		colorOut = color;
	}	

	)";

	const GLchar* fragmentShaderSource = R"(
	#version 430
	in vec3 colorOut;

	out vec4 finalColor;
	
	void main()
	{
		finalColor = vec4(colorOut, 1.0f);
	}	

	)";


	std::vector<GLuint> allShaders;

	GLuint vertexShaderId = CreateShader(vertexShaderSource, GL_VERTEX_SHADER);
	GLuint fragmentShaderId = CreateShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

	allShaders.push_back(vertexShaderId);
	allShaders.push_back(fragmentShaderId);

	programId = CreateProgram(allShaders);

}

void DisplayFunc()
{
	glClearColor(0.0f, 0.0, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programId);

	glBindVertexArray(vao);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);

	glBindVertexArray(0);

	glUseProgram(0);

	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("OpenGL Instancing Example");
	glutShowWindow();
	
 	GLenum status =	glewInit();
	if (status != GLEW_OK)
	{
		const GLubyte* errorString = glewGetErrorString(status);
		std::cout << "Error in Initializing GLEW. Error : " << errorString << std::endl;
		return -1;
	}

	CreateInstancedProgram();
	CreateVertexBuffer();
	//SetUniform();

	glutDisplayFunc(DisplayFunc);
	glutMainLoop();

	return 0;
}

