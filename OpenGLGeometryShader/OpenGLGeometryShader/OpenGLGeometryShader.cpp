#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glutils.h"

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800



float vertices[8] = {0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f};


GLuint vao, vbo, programId;

void CreateVertexBuffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void CreateProgram()
{
	const GLchar* vertexShaderSource = R"(
	#version 430
	layout(location=0) in vec2 position;
	
	void main()
	{
		gl_Position = vec4(position, 0.0f, 1.0f);
	}
	)";

	const GLchar* fragmentShaderSource = R"(
	#version 430
	in vec4 color;
	out vec4 finalColor;	

	void main()
	{
		finalColor = color;
	}
	)";


	const GLchar* geometryShaderSource = R"(
	#version 430
	layout(points) in;
	layout(line_strip, max_vertices = 5) out;

	out vec4 color;

	void main()
	{
		color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
		gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.0, 0.0, 0.0);
		EmitVertex();

		gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
		EmitVertex();

		color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
		gl_Position = gl_in[0].gl_Position + vec4(-0.1, -0.1, 0.0, 0.0);
		EmitVertex();


		EndPrimitive();		

		color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
		gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.1, 0.0, 0.0);
		EmitVertex();

		gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.1, 0.0, 0.0);
		EmitVertex();

		gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.2, 0.0, 0.0);
		EmitVertex();

		EndPrimitive();		
	}

	)";
	std::vector<GLuint> allShaders;

	GLuint vertexShaderId = CreateShader(vertexShaderSource, GL_VERTEX_SHADER);
	GLuint geometryShaderId = CreateShader(geometryShaderSource, GL_GEOMETRY_SHADER);
	GLuint fragmentShaderId = CreateShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

	allShaders.push_back(vertexShaderId);
	allShaders.push_back(geometryShaderId);
	allShaders.push_back(fragmentShaderId);

	programId = CreateProgram(allShaders);
}

void DisplayFunc()
{
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programId);

	glBindVertexArray(vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vao);
	
	glDrawArrays(GL_POINTS, 0, 4);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(0);

	glutSwapBuffers();
	glutPostRedisplay();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Geometry Shader Example");

	GLenum initVal = glewInit();
	if (initVal != GLEW_OK)
	{
		const GLubyte* errStr = glewGetErrorString(initVal);
		std::cout << "Unable to initialize GLEW. Err : " << errStr << std::endl;
	}

	std::cout << "Version OpenGL : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Version GLSL : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	CreateProgram();
	CreateVertexBuffer();

	glutDisplayFunc(DisplayFunc);
	glutMainLoop();

	return 0;
}

